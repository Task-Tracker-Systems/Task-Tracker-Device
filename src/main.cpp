#include <Arduino.h>
#include <FFat.h>
#include <atomic>
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

static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t *buffer,
                       uint32_t bufsize) {
  HWSerial.printf("MSC WRITE: lba: %u, offset: %u, bufsize: %u\n", lba, offset,
                  bufsize);
  esp_partition_write(fatPartition, offset, buffer, bufsize);
  return bufsize;
}

static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer,
                      uint32_t bufsize) {
  HWSerial.printf("MSC READ: lba: %u, offset: %u, bufsize: %u\n", lba, offset,
                  bufsize);
  esp_partition_read(fatPartition, offset, buffer, bufsize);
  return bufsize;
}

static bool onStartStop(uint8_t power_condition, bool start, bool load_eject) {
  HWSerial.printf("MSC START/STOP: power: %u, start: %u, eject: %u\n",
                  power_condition, start, load_eject);
  return true;
}

static std::atomic_bool fs_changed{false};

// Invoked when received Test Unit Ready command.
// return true allowing host to read/write this LUN e.g SD card inserted
bool msc_ready_callback(void) {
  // if fs has changed, mark unit as not ready temporarily to force PC to flush
  // cache
  return !fs_changed.exchange(false);
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
    HWSerial.println("FatFS konnte nicht gestartet werden!");
    return;
  }
  HWSerial.println("FatFS erfolgreich gemountet.");

  fatPartition = check_ffat_partition(FFAT_PARTITION_LABEL);
  if (!fatPartition) {
    printf("Error with FAT partition");
    return;
  }

  USB.onEvent(usbEventCallback);
  MSC.vendorID("ESP32");      // max 8 chars
  MSC.productID("USB_MSC");   // max 16 chars
  MSC.productRevision("1.0"); // max 4 chars
  MSC.onStartStop(onStartStop);
  MSC.onRead(onRead);
  MSC.onWrite(onWrite);
  MSC.mediaPresent(true);
  MSC.begin(DISK_SECTOR_COUNT, DISK_SECTOR_SIZE);
  USBSerial.begin();
  USB.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}
#endif /* ARDUINO_USB_MODE */
