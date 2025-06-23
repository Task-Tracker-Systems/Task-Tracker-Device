/**
 * @file
 * @brief Fake for testing on native
 */

#pragma once
#include <stdint.h>

typedef const char *esp_event_base_t;
typedef void (*esp_event_handler_t)(void *, esp_event_base_t, int32_t, void *);

enum arduino_usb_event_t
{
    ARDUINO_USB_STARTED_EVENT,
    ARDUINO_USB_STOPPED_EVENT,
};

struct ESPUSB
{
    bool begin()
    {
        return true;
    }

    void onEvent(arduino_usb_event_t, esp_event_handler_t)
    {
    }
};
extern ESPUSB USB;
