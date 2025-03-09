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

#if defined(ARDUINO_USB_MODE)
static_assert(ARDUINO_USB_MODE == 0, "USB must be in OTG mode");
#endif

const esp_partition_t *check_ffat_partition(const char *label); // defined in FFat.cpp

static USBMSC usbMsc;

static constexpr std::uint16_t blockSize = 512; // Should be 512

static const esp_partition_t *partition = nullptr;
static const char *const TAG = "STORAGE";

class ReadyCondition
{
  public:
    void setReady(const bool new_state)
    {
        ready = new_state;
        conditionVariable.notify_all();
    }
    bool isReady() const
    {
        return ready;
    }
    void wait_unitl_ready() const
    {
        std::mutex cv_m;
        std::unique_lock<std::mutex> lock(cv_m);
        conditionVariable.wait(lock, [this] { return isReady(); });
    }

  private:
    mutable std::condition_variable conditionVariable;
    std::atomic<bool> ready;
};

static ReadyCondition fileSystemState;

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

/**
 * Lists files and directories at path.
 */
static void listFiles(const char *const dirname)
{
    std::cout << "Directory: '" << dirname << "'" << std::endl;
    File root = FFat.open(dirname);
    if (!root || !root.isDirectory())
    {
        ESP_LOGE(TAG, "Error: '%s' is not a directory!\n", dirname);
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        std::cout << "\t" << file.name() << " (" << (file.isDirectory() ? "d" : "f") << ", " << file.size() << " Bytes)"
                  << std::endl;
        file.close();
        file = root.openNextFile();
    }
    file.close();
    root.close();
}

/**
 * Switch from USB MSC to application mode (file system).
 */
static void switchToApplicationMode()
{
    usbMsc.mediaPresent(false);
    FFat.end();           // invalidate cache
    assert(FFat.begin()); // update data
    fileSystemState.setReady(true);
}

/**
 * Switch from application mode (file system) to USB MSC.
 */
static void switchToUSBMode()
{
    FFat.end(); // flush and unmount
    fileSystemState.setReady(false);
    usbMsc.mediaPresent(true);
}

static void usb_stopped_cb(void *const pvParameters)
{
    switchToApplicationMode();
    listFiles("/");
    vTaskDelete(nullptr);
}

static void usb_started_cb(void *const pvParameters)
{
    switchToUSBMode();
    vTaskDelete(nullptr);
}

static bool usbIsRunning = false;
static void usbStoppedCallback(void *, esp_event_base_t, int32_t, void *)
{
    if (!usbIsRunning)
    {
        return;
    }
    usbIsRunning = false;
    xTaskCreate(usb_stopped_cb, "USB_Stopped_CB", 4096, nullptr, 5, nullptr);
}
static void usbStartedCallback(void *, esp_event_base_t, int32_t, void *)
{
    usbIsRunning = true;
    xTaskCreate(usb_started_cb, "USB_Started_CB", 4096, nullptr, 5, nullptr);
}

void Storage::begin()
{

    if (!FFat.begin(true))
    {
        ESP_LOGE(TAG, "Failed to init files system, flash may not be formatted");
        return;
    }
    ESP_LOGI(TAG, "FatFS erfolgreich gemountet.");

    partition = check_ffat_partition(FFAT_PARTITION_LABEL);
    if (!partition)
    {
        ESP_LOGE(TAG, "Error with FAT partition");
        return;
    }
    ESP_LOGI(TAG, "Flash has a size of %u bytes\n", FFat.totalBytes());

    USB.onEvent(ARDUINO_USB_STARTED_EVENT, usbStartedCallback);
    USB.onEvent(ARDUINO_USB_STOPPED_EVENT, usbStoppedCallback);
    usbMsc.vendorID("ESP32");      // max 8 chars
    usbMsc.productID("USB_MSC");   // max 16 chars
    usbMsc.productRevision("1.0"); // max 4 chars
    usbMsc.onStartStop(usbMsc_onStartStop);
    // Set callback
    usbMsc.onRead(usbMsc_onRead);
    usbMsc.onWrite(usbMsc_onWrite);
    // MSC is ready for read/write
    usbMsc.mediaPresent(true);

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

void Storage::waitForFileSystem()
{
    fileSystemState.wait_unitl_ready();
    listFiles("/");
}
