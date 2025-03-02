#include <Arduino.h>
#include <storage.hpp>

#if ARDUINO_USB_CDC_ON_BOOT == 1
#define HWSerial Serial0
#define USBSerial Serial
#else
#define HWSerial Serial
#include <USBCDC.h>
USBCDC USBSerial;
#endif

struct TestStorage : Storage
{

    /**
     * Lists files and directories at path.
     */
    static void listFiles(const char *const dirname)
    {
        auto FFat = getFileSystem();
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
};

void setup()
{
    ESP_LOGE("tag", "hello"), HWSerial.begin(115200);
    HWSerial.setDebugOutput(true);
    Storage::begin();

    USBSerial.begin();
}

void loop()
{
    // put your main code here, to run repeatedly:
    static uint32_t lastTrigger = 0;
    if (millis() - lastTrigger > 5000)
    {
        HWSerial.print(".");
        if (Storage::isFileSystemReady())
        {
            TestStorage::listFiles("/");
        }
        lastTrigger = millis();
    }
}
