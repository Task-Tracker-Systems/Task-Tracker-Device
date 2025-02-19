#include <Arduino.h>
#include <FFat.h>
#include <esp_partition.h>
#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
void setup() {}
void loop() {}
#else
#include "USB.h"
#include "USBMSC.h"

#if ARDUINO_USB_CDC_ON_BOOT
#define HWSerial Serial0
#define USBSerial Serial
#else
#define HWSerial Serial
USBCDC USBSerial;
#endif

USBMSC MSC;

static const uint32_t DISK_SECTOR_COUNT =
    2 * 8; // 8KB is the smallest size that windows allow to mount
static const uint16_t DISK_SECTOR_SIZE = 512; // Should be 512
static const uint16_t DISC_SECTORS_PER_TABLE =
    1; // each table sector can fit 170KB (340 sectors)

static const esp_partition_t *fatPartition = nullptr;

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t *buffer,
                       uint32_t bufsize) {
  HWSerial.printf("MSC WRITE: lba: %u, offset: %u, bufsize: %u\n", lba, offset,
                  bufsize);
  esp_partition_write(fatPartition, offset, buffer,
                      bufsize); // TODO handle error codes
  return bufsize;
}

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer,
                      uint32_t bufsize) {
  HWSerial.printf("MSC READ: lba: %u, offset: %u, bufsize: %u\n", lba, offset,
                  bufsize);
  esp_partition_read(fatPartition, offset, buffer,
                     bufsize); // TODO handle error codes
  return bufsize;
}

static bool onStartStop(uint8_t power_condition, bool start, bool load_eject) {
  HWSerial.printf("MSC START/STOP: power: %u, start: %u, eject: %u\n",
                  power_condition, start, load_eject);
  return true;
}

static void refreshMassStorage(void) { MSC.mediaPresent(false); }

bool exists(String path) {
  bool yes = false;
  File32 file = fatfs.open(path, O_READ);
  if (file && !file.isDirectory()) {
    yes = true;
  }
  file.close();
  return yes;
}

static void rebootFs() {
  FFat.end();
  FFat.begin();
}

// Callback invoked when WRITE10 command is completed (status received and
// accepted by host). used to flush any pending cache.
void tud_msc_write10_complete_cb(void) {
  MSC.mediaPresent(true);
  // sync with flash
  rebootFs();

  // clear file system's cache to force refresh
  fatfs.cacheClear();

#ifdef LED_BUILTIN
  digitalWrite(LED_BUILTIN, LOW);
#endif
}

static void usbEventCallback(void *arg, esp_event_base_t event_base,
                             int32_t event_id, void *event_data) {
  if (event_base == ARDUINO_USB_EVENTS) {
    arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
    switch (event_id) {
    case ARDUINO_USB_STARTED_EVENT:
      HWSerial.println("USB PLUGGED");
      break;
    case ARDUINO_USB_STOPPED_EVENT:
      HWSerial.println("USB UNPLUGGED");
      break;
    case ARDUINO_USB_SUSPEND_EVENT:
      HWSerial.printf("USB SUSPENDED: remote_wakeup_en: %u\n",
                      data->suspend.remote_wakeup_en);
      break;
    case ARDUINO_USB_RESUME_EVENT:
      HWSerial.println("USB RESUMED");
      break;

    default:
      break;
    }
  }
}

const esp_partition_t *
check_ffat_partition(const char *label); // defined in FFat.cpp

void setup() {
  HWSerial.begin(115200);
  HWSerial.setDebugOutput(true);

  if (!FFat.begin(
          true)) { // `true` = Formatieren falls kein Dateisystem vorhanden
    HWSerial.println("Failed to init files system, flash may not be formatted");
    return;
  }
  HWSerial.println("FatFS erfolgreich gemountet.");

  fatPartition = check_ffat_partition(FFAT_PARTITION_LABEL);
  if (!fatPartition) {
    printf("Error with FAT partition");
    return;
  }
  HWSerial.printf("Flash has a size of %u bytes\n", FFat.totalBytes());

  USB.onEvent(usbEventCallback);
  MSC.vendorID("ESP32");      // max 8 chars
  MSC.productID("USB_MSC");   // max 16 chars
  MSC.productRevision("1.0"); // max 4 chars
  MSC.onStartStop(onStartStop);
  MSC.onRead(onRead);
  MSC.onWrite(onWrite);
  // Set callback
  usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
  // MSC is ready for read/write
  MSC.mediaPresent(true);

  // Set disk size, block size should be 512 regardless of spi flash page size
  MSC.begin(FFat.totalBytes() / DISK_SECTOR_SIZE, DISK_SECTOR_SIZE);
  USBSerial.begin();
  USB.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}
#endif /* ARDUINO_USB_MODE */
