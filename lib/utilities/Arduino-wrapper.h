/**
 * \file .
 *
 * Circumvents problematic processor macros defined by Arduino framework.
 *
 * Some of these problems may not occur in all Arduino framework implementations.
 */

#pragma once
#include <Arduino.h>

#if defined(round)
#undef round //see https://github.com/esp8266/Arduino/issues/5787#issuecomment-465852231
#endif
#if defined(abs)
#undef abs // else it conflicts with std::abs
#endif

#if defined(ArduinoFake)
#if defined(digitalPinToInterrupt)
// the definition in ArduinoFake is too primitive for testing
#undef digitalPinToInterrupt
#define digitalPinToInterrupt(pinNumber) pinNumber
#endif
#endif
