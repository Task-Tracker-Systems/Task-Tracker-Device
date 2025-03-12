#include <Arduino.h>

#include "storage.hpp"
#include <FFat.h>
#include <USB.h>
#include <USBMSC.h>
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <esp32-hal-log.h>
#include <esp_err.h>
#include <esp_partition.h>
#include <iostream>
#include <mutex>
#include <thread>

#if defined(ARDUINO_USB_MODE)
static_assert(ARDUINO_USB_MODE == 0, "USB must be in OTG mode");
#endif

const esp_partition_t *check_ffat_partition(const char *label); // defined in FFat.cpp

static USBMSC usbMsc;

static constexpr std::uint16_t blockSize = 512; // Should be 512

static const esp_partition_t *partition = nullptr;
static const char *const TAG = "STORAGE";

static std::condition_variable stateChangeRequested;
static std::condition_variable stateChanged;
static std::atomic<bool> requestFileSystemActive;
static std::thread stateMachine;
static std::mutex fileSystemState_mutex; //!< used to lock the actual state
static bool fileSystemIsActive;          //!< describes the current state

std::shared_ptr<fs::FS> Storage::getFileSystem_locking()
{
    std::unique_lock fs_state_lock{fileSystemState_mutex};
    if (!fileSystemIsActive)
    {
        stateChanged.wait(fs_state_lock, []() { return fileSystemIsActive; });
    }
    fs_state_lock.release();
    return {&FFat, [](fs::FS *) { fileSystemState_mutex.unlock(); }};
}

static void requestState(const bool fileSystemActive)
{
    requestFileSystemActive = fileSystemActive;
    ESP_LOGD(TAG, "request new state: %s", fileSystemActive ? "true" : "false");
    stateChangeRequested.notify_all();
}

static void processStateRequests()
{
    while (true)
    {
        std::unique_lock fs_state_lock{fileSystemState_mutex};
        ESP_LOGD(TAG, "waiting for state change request");
        stateChangeRequested.wait(fs_state_lock, []() { return requestFileSystemActive != fileSystemIsActive; });
        if (fileSystemIsActive = requestFileSystemActive)
        {
            ESP_LOGI(TAG, "mount FS");
            usbMsc.mediaPresent(false);
            FFat.end();           // invalidate cache
            assert(FFat.begin()); // update data
        }
        else
        {
            ESP_LOGI(TAG, "unmount FS");
            FFat.end(); // flush and unmount
            usbMsc.mediaPresent(true);
        }
        stateChanged.notify_all();
    }
}

/**
 * Callback invoked when received WRITE10 command.
 *
 * Process data in buffer to disk's storage.
 *
 * @param lba logical block address
 * @returns the number of written bytes (must be multiple of block size)
 */
static std::int32_t usbMsc_onWrite(const std::uint32_t lba, const std::uint32_t offset, std::uint8_t *const buffer,
                                   const uint32_t bufsize)
{
    ESP_LOGV(TAG, "MSC WRITE: lba: %u, offset: %u, bufsize: %u\n", lba, offset, bufsize);
    const std::uint32_t byteOffset = lba * blockSize + offset;
    ESP_ERROR_CHECK(esp_partition_erase_range(partition, byteOffset, bufsize)); // erase must be called before write
    ESP_ERROR_CHECK(esp_partition_write(partition, byteOffset, buffer, bufsize));
    return bufsize;
}

/**
 * Callback invoked when received READ10 command.
 *
 * Copy disk's data to buffer (up to bufsize).
 *
 * @param lba logical block address
 * @returns the number of copied bytes (must be multiple of block size)
 */
static std::int32_t usbMsc_onRead(const std::uint32_t lba, const std::uint32_t offset, void *const buffer,
                                  const std::uint32_t bufsize)
{
    ESP_LOGV(TAG, "MSC READ: lba: %u, offset: %u, bufsize: %u\n", lba, offset, bufsize);
    const std::uint32_t byteOffset = lba * blockSize + offset;
    ESP_ERROR_CHECK(esp_partition_read(partition, byteOffset, buffer, bufsize));
    return bufsize;
}

static bool usbMsc_onStartStop(const std::uint8_t power_condition, const bool start, const bool load_eject)
{
    ESP_LOGV(TAG, "MSC START/STOP: power: %u, start: %u, eject: %u\n", power_condition, start, load_eject);
    return true;
}

std::size_t Storage::size()
{
    return FFat.totalBytes();
}

static std::atomic<bool> usbIsRunning = false;
static void usbStoppedCallback(void *, esp_event_base_t, int32_t, void *)
{
    if (!usbIsRunning)
    {
        return;
    }
    usbIsRunning = false;
    requestState(true);
}
static void usbStartedCallback(void *, esp_event_base_t, int32_t, void *)
{
    usbIsRunning = true;
    requestState(false);
}

void Storage::begin()
{

    if (!FFat.begin(true))
    {
        ESP_LOGE(TAG, "Failed to init files system, flash may not be formatted");
        return;
    }
    ESP_LOGI(TAG, "file system initialized");

    partition = check_ffat_partition(FFAT_PARTITION_LABEL);
    if (!partition)
    {
        ESP_LOGE(TAG, "Error with FAT partition");
        return;
    }
    ESP_LOGI(TAG, "Flash has a size of %u bytes\n", FFat.totalBytes());

    // define state before callbacks are activated
    fileSystemIsActive = true;
    requestFileSystemActive = true;
    stateMachine = std::thread(processStateRequests);

    usbMsc.vendorID("ESP32");      // max 8 chars
    usbMsc.productID("USB_MSC");   // max 16 chars
    usbMsc.productRevision("1.0"); // max 4 chars
    usbMsc.onStartStop(usbMsc_onStartStop);
    usbMsc.mediaPresent(false);
    // Set callback
    usbMsc.onRead(usbMsc_onRead);
    usbMsc.onWrite(usbMsc_onWrite);
    USB.onEvent(ARDUINO_USB_STARTED_EVENT, usbStartedCallback);
    USB.onEvent(ARDUINO_USB_STOPPED_EVENT, usbStoppedCallback);

    // Set disk size, block size should be 512 regardless of spi flash page size
    if (!usbMsc.begin(FFat.totalBytes() / blockSize, blockSize))
    {
        ESP_LOGE(TAG, "starting USB MSC failed");
    }
    if (!USB.begin())
    {
        ESP_LOGE(TAG, "starting USB failed");
    }
}

void Storage::end()
{
    usbMsc.end();
    usbIsRunning = false;
    FFat.end();
}
