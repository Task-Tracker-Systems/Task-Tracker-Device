#include <Arduino.h>
#include <esp32-hal-log.h>
#include <iostream>
#include <storage.hpp>

#if ARDUINO_USB_CDC_ON_BOOT == 1
#define HWSerial Serial0
#define USBSerial Serial
#else
#define HWSerial Serial
#include <USBCDC.h>
USBCDC USBSerial;
#endif

static const char *const TAG = "MAIN";

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
            ESP_LOGE(TAG, "'%s' is not a directory!", dirname);
            root.close();
            return;
        }

        File file = root.openNextFile();
        while (file)
        {
            HWSerial.printf("  %s (%s, %d Bytes)\n", file.name(), file.isDirectory() ? "d" : "f", file.size());
            file.close();
            file = root.openNextFile();
        }
        file.close();
        root.close();
        HWSerial.println("end of directory");
    }
};

void setup()
{
    HWSerial.begin(115200);
    HWSerial.setDebugOutput(true);
    delay(300); // in order to give the serial monitor time to start
    HWSerial.println("START HWSerial");
    ESP_LOGE(TAG, "Example error");
    ESP_LOGW(TAG, "Example warning");
    ESP_LOGI(TAG, "Example info");
    ESP_LOGD(TAG, "Example debug");
    ESP_LOGV(TAG, "Example verbose");
    std::cout << "Hello" << std::endl;
    Storage::begin(true);
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
