#include "KeyInputs.hpp"
#include "lvgl.h"
#include "user_interaction/IKeypad.hpp"
#include <iterator>

static IKeypad *myKeypad;

static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

void RegisterKeyInputs(IKeypad *keypad)
{
    //assign keypad reference to local pointer
    myKeypad = keypad;

    // Register at least one display before you register any input devices
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = keypad_read;
    // Register the driver in LVGL and save the created input device object
    lv_indev_drv_register(&indev_drv);
}

static constexpr KeyId keysToSearch[] = {KeyId::LEFT, KeyId::RIGHT, KeyId::ENTER, KeyId::BACK};

static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static auto lastCheckedKeyIndex = 0;

    // assign key state
    data->state = (myKeypad->isKeyPressed(keysToSearch[lastCheckedKeyIndex])) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

    // find correct lvgl key for KeyId
    switch (keysToSearch[lastCheckedKeyIndex])
    {
    case KeyId::LEFT:
        data->key = LV_KEY_LEFT;
        break;
    case KeyId::RIGHT:
        data->key = LV_KEY_RIGHT;
        break;
    case KeyId::ENTER:
        data->key = LV_KEY_ENTER;
        break;
    case KeyId::BACK:
        data->key = LV_KEY_ESC;
        break;
    }

    // increment Key Index and reset if all keys were examined
    if (++lastCheckedKeyIndex >= std::size(keysToSearch))
    {
        lastCheckedKeyIndex = 0;
        data->continue_reading = false;
    }
    else
    {
        data->continue_reading = true;
    }

    LV_LOG_USER("%hu key:%-4lu  state:%-2lu", data->continue_reading, data->key, data->state);
}
