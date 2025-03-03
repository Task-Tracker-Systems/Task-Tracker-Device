#include "storage.hpp"

#include <Arduino.h>
#if defined(ARDUINO_USB_MODE)
static_assert(ARDUINO_USB_MODE == 0, "must be used when USB is in OTG mode");
#endif
#include <FFat.h>
#include <USB.h>
#include <USBMSC.h>
#include <esp_err.h>
#include <esp_partition.h>

#if ARDUINO_USB_CDC_ON_BOOT == 1
#define HWSerial Serial0
#else
#define HWSerial Serial
#endif

static USBMSC MSC;

static const uint32_t DISK_SECTOR_COUNT = 2 * 8;  // 8KB is the smallest size that windows allow to mount
static const uint16_t DISK_SECTOR_SIZE = 512;     // Should be 512
static const uint16_t DISC_SECTORS_PER_TABLE = 1; // each table sector can fit 170KB (340 sectors)

static const esp_partition_t *fatPartition = nullptr;

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize)
{
    HWSerial.printf("MSC WRITE: lba: %u, offset: %u, bufsize: %u\n", lba, offset, bufsize);
    uint32_t byteOffset = lba * DISK_SECTOR_SIZE + offset;
    // erase must be called before write
    ESP_ERROR_CHECK(esp_partition_erase_range(fatPartition, byteOffset, bufsize));
    ESP_ERROR_CHECK(esp_partition_write(fatPartition, byteOffset, buffer, bufsize));
    return bufsize;
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
    HWSerial.printf("MSC READ: lba: %u, offset: %u, bufsize: %u\n", lba, offset, bufsize);
    uint32_t byteOffset = lba * DISK_SECTOR_SIZE + offset;
    ESP_ERROR_CHECK(esp_partition_read(fatPartition, byteOffset, buffer, bufsize));
    return bufsize;
}

static bool onStartStop(uint8_t power_condition, bool start, bool load_eject)
{
    HWSerial.printf("MSC START/STOP: power: %u, start: %u, eject: %u\n", power_condition, start, load_eject);
    return true;
}

/**
 * Lists files and directories at path.
 */
static void listFiles(const char *const dirname)
{
    HWSerial.printf("Directory: '%s'\n", dirname);
    File root = FFat.open(dirname);
    if (!root || !root.isDirectory())
    {
        HWSerial.printf("Error: '%s' is not a directory!\n", dirname);
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        HWSerial.printf("  %s (%s, %d Bytes)\n", file.name(), file.isDirectory() ? "d" : "f", file.size());
        file = root.openNextFile();
    }
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

static void usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == ARDUINO_USB_EVENTS)
    {
        arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
        static int32_t old_event_id = ARDUINO_USB_ANY_EVENT;
        if (old_event_id == event_id)
            return;
        switch (event_id)
        {
        case ARDUINO_USB_STARTED_EVENT:
            HWSerial.println("USB PLUGGED");
            xTaskCreate(usb_started_cb, "USB_Started_CB", 4096, nullptr, 5, nullptr);
            break;
        case ARDUINO_USB_STOPPED_EVENT:
            HWSerial.println("USB UNPLUGGED");
            xTaskCreate(usb_stopped_cb, "USB_Stopped_CB", 4096, nullptr, 5, nullptr);
            break;
        case ARDUINO_USB_SUSPEND_EVENT:
            HWSerial.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en);
            break;
        case ARDUINO_USB_RESUME_EVENT:
            HWSerial.println("USB RESUMED");
            break;

        default:
            break;
        }
        old_event_id = event_id;
    }
}

const esp_partition_t *check_ffat_partition(const char *label); // defined in FFat.cpp

void Storage::begin()
{

    if (!FFat.begin(true))
    { // `true` = Formatieren falls kein Dateisystem vorhanden
        HWSerial.println("Failed to init files system, flash may not be formatted");
        return;
    }
    HWSerial.println("FatFS erfolgreich gemountet.");

    fatPartition = check_ffat_partition(FFAT_PARTITION_LABEL);
    if (!fatPartition)
    {
        printf("Error with FAT partition");
        return;
    }
    HWSerial.printf("Flash has a size of %u bytes\n", FFat.totalBytes());

    USB.onEvent(usbEventCallback);
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
    MSC.begin(FFat.totalBytes() / DISK_SECTOR_SIZE, DISK_SECTOR_SIZE);
    USB.begin();
}
