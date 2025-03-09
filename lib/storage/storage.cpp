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

/**
 * Lists files and directories at path.
 */
static void listFiles(const char *const dirname, const std::shared_ptr<fs::FS> fs)
{
    std::cout << "Directory: '" << dirname << "'" << std::endl;
    File root = fs->open(dirname);
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

class FileSystemSwitcher
{
  public:
    void begin(const bool fsIsActive)
    {
        fileSystemIsActive = fsIsActive;
        requestFileSystemActive = fsIsActive;
        stateMachine = std::thread(&FileSystemSwitcher::processStateRequests, this);
    }

    std::shared_ptr<fs::FS> getFileSystem_locking()
    {
        std::unique_lock fs_state_lock{fileSystemState_mutex};
        if (!fileSystemIsActive)
        {
            stateChanged.wait(fs_state_lock, [this]() { return fileSystemIsActive; });
        }
        fs_state_lock.release();
        return {&FFat, [this](fs::FS *) { fileSystemState_mutex.unlock(); }};
    }
    void requestState(const bool fileSystemActive)
    {
        requestFileSystemActive = fileSystemActive;
        ESP_LOGD(TAG, "request new state: %s", fileSystemActive ? "true" : "false");
        stateChangeRequested.notify_all();
    }

  protected:
    bool isFileSystemActive() const
    {
        return fileSystemIsActive;
    }
    void processStateRequests()
    {
        while (true)
        {
            std::unique_lock fs_state_lock{fileSystemState_mutex};
            ESP_LOGD(TAG, "waiting for state change request");
            stateChangeRequested.wait(fs_state_lock,
                                      [this]() { return requestFileSystemActive != fileSystemIsActive; });
            if (fileSystemIsActive = requestFileSystemActive)
            {
                ESP_LOGD(TAG, "mount FS");
                usbMsc.mediaPresent(false);
                FFat.end();           // invalidate cache
                assert(FFat.begin()); // update data
            }
            else
            {
                ESP_LOGD(TAG, "unmount FS");
                FFat.end(); // flush and unmount
                usbMsc.mediaPresent(true);
            }
            stateChanged.notify_all();
        }
    }

  private:
    std::condition_variable stateChangeRequested;
    std::condition_variable stateChanged;
    std::atomic<bool> requestFileSystemActive;
    std::thread stateMachine;
    std::mutex fileSystemState_mutex; //!< used to lock the actual state
    bool fileSystemIsActive;          //!< describes the current state
};

static FileSystemSwitcher fileSystemSwitcher;

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
    fileSystemSwitcher.requestState(true);
}
static void usbStartedCallback(void *, esp_event_base_t, int32_t, void *)
{
    usbIsRunning = true;
    fileSystemSwitcher.requestState(false);
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

    fileSystemSwitcher.begin(true); // define state before callbacks are activated

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

void Storage::test()
{
    auto fs_p = fileSystemSwitcher.getFileSystem_locking();
    listFiles("/", fs_p);
}
