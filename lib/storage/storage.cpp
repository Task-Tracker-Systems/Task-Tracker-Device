#include "storage.hpp"
#include <FFat.h>
#include <USB.h>
#include <USBMSC.h>
#include <cstdint>
#include <iostream>
#include <string>

#if defined(ARDUINO_USB_MODE)
static_assert(ARDUINO_USB_MODE == 0, "USB must be in OTG mode");
#endif

const esp_partition_t *check_ffat_partition(const char *label); // defined in FFat.cpp

static const std::string rootPath = "/";
static constexpr std::size_t blockSize = 512; // bytes

static const esp_partition_t *partition;
static USBMSC usbMsc;
static bool usbIsRunning = false;
static bool fileSystemIsReady = false;

static void callbackUsbStarted(void *, esp_event_base_t, int32_t event_id, void *)
{
    usbIsRunning = true;
    Storage::switchToUsbMode();
}

static void callbackUsbStopped(void *, esp_event_base_t, int32_t event_id, void *)
{
    if (!usbIsRunning) /* https://github.com/espressif/arduino-esp32/issues/7228 */
    {
        return;
    }
    usbIsRunning = false;
    Storage::switchToApplicationMode();
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
    std::cout << "MSC WRITE: lba: " << lba << ", offset: " << offset << ", bufsize: " << bufsize << std::endl;
    const std::uint32_t byteOffset = lba * blockSize + offset;
    ESP_ERROR_CHECK(esp_partition_erase_range(partition, byteOffset, bufsize)); // erase must be called before write
    ESP_ERROR_CHECK(esp_partition_write(partition, byteOffset, buffer, bufsize));
    return bufsize;
}

//  and
// return

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
    std::cout << "MSC READ: lba: " << lba << ", offset: " << offset << ", bufsize: " << bufsize << std::endl;
    const std::uint32_t byteOffset = lba * blockSize + offset;
    ESP_ERROR_CHECK(esp_partition_read(partition, byteOffset, buffer, bufsize));
    return bufsize;
}

static bool usbMsc_onStartStop(const std::uint8_t power_condition, const bool start, const bool load_eject)
{
    std::cout << "MSC START/STOP: power: " << power_condition << ", start: " << start << ", eject: " << load_eject
              << std::endl;
    return true;
}

std::size_t Storage::size()
{
    return FFat.totalBytes();
}

void Storage::begin(const bool formatFsOnFail, const char *const partitionLabel)
{
    partition = check_ffat_partition(partitionLabel);

    if (!partition)
    {
        std::cerr << "Error with partition!" << std::endl;
        return;
    }

    // initialize file system
    const auto basePath = rootPath + partitionLabel;
    constexpr auto maxOpenFiles = 10U;
    if (!FFat.begin(formatFsOnFail, basePath.c_str(), maxOpenFiles, partitionLabel))
    {
        std::cerr << "File-system initialization failed!" << std::endl;
        return;
    }
    std::cout << "Storage has a size of " << size() << " bytes." << std::endl;

    // setup USB Mass Storage Class
    usbMsc.vendorID("TTS");        // max 8 chars
    usbMsc.productID("TTD");       // max 16 chars
    usbMsc.productRevision("1.0"); // max 4 chars
    usbMsc.onStartStop(usbMsc_onStartStop);
    // Set callback
    usbMsc.onRead(usbMsc_onRead);
    usbMsc.onWrite(usbMsc_onWrite);
    // usbMsc is ready for read/write
    usbMsc.mediaPresent(true);
    usbMsc.begin(FFat.totalBytes() / blockSize, blockSize);
    fileSystemIsReady = true;

    // subscribe to USB events
    USB.onEvent(ARDUINO_USB_STARTED_EVENT, callbackUsbStarted);
    USB.onEvent(ARDUINO_USB_STOPPED_EVENT, callbackUsbStopped);
    USB.begin();
}

void Storage::end()
{
    usbMsc.end();
    usbIsRunning = false;
    FFat.end();
    fileSystemIsReady = false;
}

void Storage::switchToUsbMode()
{
    FFat.end(); // flush and unmount
    fileSystemIsReady = false;
    usbMsc.mediaPresent(true);
}

bool Storage::isFileSystemReady()
{
    return fileSystemIsReady;
}

void Storage::switchToApplicationMode()
{
    usbMsc.mediaPresent(false);
    FFat.end();   // invalidate cache
    FFat.begin(); // update data
    fileSystemIsReady = true;
}

fs::FS &Storage::getFileSystem()
{
    return FFat;
}
