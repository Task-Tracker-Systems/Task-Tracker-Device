#include <Arduino.h>
#if defined(ARDUINO_USB_MODE)
static_assert(ARDUINO_USB_MODE == 0, "must be used when USB is in OTG mode");
#endif
#include <USB.h>
#include <USBMSC.h>
#include <esp_err.h>
#include <storage.hpp>

#if ARDUINO_USB_CDC_ON_BOOT == 1
#define HWSerial Serial0
#else
#define HWSerial Serial
#endif

void setup()
{
    HWSerial.begin(115200);
    HWSerial.setDebugOutput(true);
    Storage::begin();
}

void loop()
{
    // put your main code here, to run repeatedly:
    static uint32_t lastTrigger = 0;
    if (millis() - lastTrigger > 1000)
    {
        HWSerial.print(".");
        lastTrigger = millis();
    }
}
