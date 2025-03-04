#include <Arduino.h>
#if defined(ARDUINO_USB_MODE)
static_assert(ARDUINO_USB_MODE == 0, "must be used when USB is in OTG mode");
#endif
#include <USB.h>
#include <USBMSC.h>
#include <esp32-hal-log.h>
#include <esp_err.h>
#include <iostream>
#include <storage.hpp>

#if ARDUINO_USB_CDC_ON_BOOT == 1
#define HWSerial Serial0
#else
#define HWSerial Serial
#endif

static const char *const TAG = "MAIN";

void setup()
{
    HWSerial.begin(115200);
    delay(200); // wait for the serial monitor to be ready
    HWSerial.setDebugOutput(true);
    ESP_LOGV(TAG, "verbose");
    ESP_LOGD(TAG, "debug");
    ESP_LOGI(TAG, "info");
    ESP_LOGW(TAG, "warning");
    ESP_LOGE(TAG, "error");
    std::cout << "Started program" << std::endl;
    Storage::begin();
}

void loop()
{
    // put your main code here, to run repeatedly:
    static uint32_t lastTrigger = 0;
    if (millis() - lastTrigger > 1000)
    {
        std::cout << "." << std::flush;
        lastTrigger = millis();
    }
}
