/**
 * @file
 * @brief Fake for testing on native
 */

#pragma once
#include <stdint.h>

typedef bool (*msc_start_stop_cb)(uint8_t, bool, bool);
typedef int32_t (*msc_read_cb)(uint32_t, uint32_t, void *, uint32_t);
typedef int32_t (*msc_write_cb)(uint32_t, uint32_t, uint8_t *, uint32_t);

struct USBMSC
{
    bool begin(uint32_t, uint16_t)
    {
        return true;
    }
    void end()
    {
    }
    void vendorID(const char *)
    {
    }
    void productID(const char *)
    {
    }
    void productRevision(const char *)
    {
    }
    void mediaPresent(bool)
    {
    }
    void onStartStop(msc_start_stop_cb);
    void onRead(msc_read_cb);
    void onWrite(msc_write_cb);
};
