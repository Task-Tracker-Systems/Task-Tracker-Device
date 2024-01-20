#pragma once
#include <Arduino.h>

// this is only relevant for Arduino from ArduinoFake
// problem: https://github.com/esp8266/Arduino/issues/5787
// not-esp32 problem: https://github.com/espressif/arduino-esp32/blob/114965010529c004ce914fea773095274ea2ce4d/cores/esp32/Arduino.h#L200

#if defined(round)
#undef round //see https://github.com/esp8266/Arduino/issues/5787#issuecomment-465852231
#endif
#if defined(abs)
#undef abs // else it conflicts with std::abs
#endif
