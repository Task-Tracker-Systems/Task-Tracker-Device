#include <Arduino.h>
#include <FS.h>
#include <chrono>
#include <esp32-hal-log.h>
#include <esp_err.h>
#include <iostream>
#include <storage.hpp>
#include <thread>

using namespace std::chrono_literals;

#if ARDUINO_USB_CDC_ON_BOOT == 1
#define HWSerial Serial0
#else
#define HWSerial Serial
#endif

static const char *const TAG = "MAIN";

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

void setup()
{
    HWSerial.begin(115200);
    HWSerial.setDebugOutput(true);
    delay(3000); // in order to give the serial monitor time to start
    std::cout << "Started program" << std::endl;
    ESP_LOGE(TAG, "Example error");
    ESP_LOGW(TAG, "Example warning");
    ESP_LOGI(TAG, "Example info");
    ESP_LOGD(TAG, "Example debug");
    ESP_LOGV(TAG, "Example verbose");
    Storage::begin();
}

void loop()
{
    listFiles("/", Storage::getFileSystem_locking());
    std::this_thread::sleep_for(3s);
}
