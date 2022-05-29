#include "main.hpp"
#include "display.h"
#include <Arduino.h>

/////////////////////////////////////////////////////////////////////////////////

const int dataPin = 34;   /* Q7 */
const int clockPin = 23;  /* CP */
constexpr int latchPin = 12;  /* PL */

constexpr int numBits = 8;   /* Set to 8 * number of shift registers */

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
    // Step 1: Sample
    digitalWrite(pin_latch, LOW);
    delayMicroseconds(1);
    digitalWrite(pin_latch, HIGH);

    // Step 2: Shift
    for (int i = 0; i < numBits; i++) {
      digitalWrite(pin_data, (data & (1u << i)) ? HIGH : LOW);
      digitalWrite(pin_clock, HIGH); // Shift out the next bit
      delayMicroseconds(1); // at least 200ns
      digitalWrite(pin_clock, LOW);
    }
  }

}

namespace main{
void setup(char const *)
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  setup_display();
  OutputShiftRegister::setup();
}
void loop()
{
  // Step 1: Sample
  digitalWrite(latchPin, LOW);
  digitalWrite(latchPin, HIGH);

  // Step 2: Shift
  Serial.print("Bits: ");
  for (int i = 0; i < numBits; i++) {
    int bit = digitalRead(dataPin);
    if (bit == HIGH) {
      Serial.print("1");
    } else {
      Serial.print("0");
    }
    digitalWrite(clockPin, HIGH); // Shift out the next bit
    digitalWrite(clockPin, LOW);
  }

  Serial.println();
  delay(200);

  static uint8_t output = 1;
  OutputShiftRegister::setRegister(output);
  if(output & (1<<7)) output = 1;
  else output <<= 1;

  testanimate(); // Animate bitmaps
}
}
