#include <Arduino.h>
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
    HWSerial.setDebugOutput(true);
    delay(300); // in order to give the serial monitor time to start
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
    // put your main code here, to run repeatedly:
    static uint32_t lastTrigger = 0;
    if (millis() - lastTrigger > 1000)
    {
        std::cout << "." << std::flush;
        lastTrigger = millis();
    }
}
