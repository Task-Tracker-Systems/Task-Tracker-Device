#include "main.hpp"
#include "display.h"
#include <Arduino.h>

namespace OutputShiftRegister
{
  constexpr int pin_data = 14;
  constexpr int pin_latch = 13;
  constexpr int pin_clock = 5;

  void setup()
  {
    pinMode(pin_data, OUTPUT);
    pinMode(pin_latch, OUTPUT);
    pinMode(pin_clock, OUTPUT);
  }

  void setRegister(const uint8_t data)
  {
    digitalWrite(pin_latch, LOW);
    shiftOut(pin_data, pin_clock, LSBFIRST, data);
    digitalWrite(pin_latch, HIGH);
  }
}

namespace InputShiftRegister
{
  constexpr int pin_data = 34;
  constexpr int pin_latch = 12;
  constexpr int pin_clock = 23;

  void setup()
  {
    pinMode(pin_data, INPUT);
    pinMode(pin_latch, OUTPUT);
    pinMode(pin_clock, OUTPUT);
  }

  typedef decltype(shiftIn(0,0,LSBFIRST)) RegisterType;

  RegisterType getRegister()
  {
    digitalWrite(pin_latch, LOW);
    delayMicroseconds(1); // at least 200ns
    digitalWrite(pin_latch, HIGH);
    return shiftIn(pin_data, pin_clock, LSBFIRST);
  }
}

namespace main{
void setup(char const *)
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  setup_display();
  OutputShiftRegister::setup();
  InputShiftRegister::setup();
}
void loop()
{
  OutputShiftRegister::setRegister(InputShiftRegister::getRegister());
  delay(100);
  testanimate(); // Animate bitmaps
}
}
