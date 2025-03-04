#include "storage.hpp"

#include <Arduino.h>
#if defined(ARDUINO_USB_MODE)
static_assert(ARDUINO_USB_MODE == 0, "must be used when USB is in OTG mode");
#endif
#include <FFat.h>
#include <USB.h>
#include <USBMSC.h>
#include <cstdint>
#include <esp_err.h>
#include <esp_partition.h>
#include <iostream>

#if ARDUINO_USB_CDC_ON_BOOT == 1
#define HWSerial Serial0
#else
#define HWSerial Serial
#endif

static USBMSC MSC;

static constexpr std::uint16_t blockSize = 512; // Should be 512

static const esp_partition_t *partition = nullptr;
static const char *const TAG = "STORAGE";

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize)
{
    ESP_LOGV(TAG, "MSC WRITE: lba: %u, offset: %u, bufsize: %u\n", lba, offset, bufsize);
    uint32_t byteOffset = lba * blockSize + offset;
    // erase must be called before write
    ESP_ERROR_CHECK(esp_partition_erase_range(partition, byteOffset, bufsize));
    ESP_ERROR_CHECK(esp_partition_write(partition, byteOffset, buffer, bufsize));
    return bufsize;
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
    ESP_LOGV(TAG, "MSC READ: lba: %u, offset: %u, bufsize: %u\n", lba, offset, bufsize);
    uint32_t byteOffset = lba * blockSize + offset;
    ESP_ERROR_CHECK(esp_partition_read(partition, byteOffset, buffer, bufsize));
    return bufsize;
}

static bool onStartStop(uint8_t power_condition, bool start, bool load_eject)
{
    ESP_LOGV(TAG, "MSC START/STOP: power: %u, start: %u, eject: %u\n", power_condition, start, load_eject);
    return true;
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
    MSC.mediaPresent(false);
    FFat.end();   // invalidate cache
    FFat.begin(); // update data
}

/**
 * Switch from application mode (file system) to USB MSC.
 */
static void switchToUSBMode()
{
    FFat.end(); // flush and unmount
    MSC.mediaPresent(true);
}

static bool usbIsRunning = false;
static void usbStoppedCallback(void *, esp_event_base_t, int32_t, void *)
{
    if (!usbIsRunning)
    {
        return;
    }
    usbIsRunning = false;
    switchToApplicationMode();
    listFiles("/");
}

static void usbStartedCallback(void *, esp_event_base_t, int32_t, void *)
{
    usbIsRunning = true;
    switchToUSBMode();
}

const esp_partition_t *check_ffat_partition(const char *label); // defined in FFat.cpp

void Storage::begin()
{

    if (!FFat.begin(true))
    { // `true` = Formatieren falls kein Dateisystem vorhanden
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
    MSC.vendorID("ESP32");      // max 8 chars
    MSC.productID("USB_MSC");   // max 16 chars
    MSC.productRevision("1.0"); // max 4 chars
    MSC.onStartStop(onStartStop);
    // Set callback
    MSC.onRead(onRead);
    MSC.onWrite(onWrite);
    // MSC is ready for read/write
    MSC.mediaPresent(true);

    // Set disk size, block size should be 512 regardless of spi flash page size
    if (!MSC.begin(FFat.totalBytes() / blockSize, blockSize))
    {
        ESP_LOGE(TAG, "starting USB MSC failed");
    }
    if (!USB.begin())
    {
        ESP_LOGE(TAG, "starting USB failed");
    }
}
