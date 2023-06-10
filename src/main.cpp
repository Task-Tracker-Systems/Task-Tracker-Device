#include "main.hpp"
#include "Controller.hpp"
#include "ProcessHmiInputs.hpp"
#include "controller_factory_interface.hpp"
#include "display.h"
#include "pitches.hpp"
#include "presenter_factory_interface.hpp"
#include <Arduino.h>
#include <board_config.hpp>
#include <cstdint>

namespace main
{
void setup(char const *programIdentificationString)
{
    pinMode(board::buzzer::pin::on_off, OUTPUT);
    setup_display();
    Serial.begin(115200);
    delay(100);
    Serial.flush();
    delay(100);
    Serial.printf("\n begin program '%s'\n", programIdentificationString);
}
void loop()
{
    static ProcessHmiInputs processHmiInputs(hmi::getController(), hmi::getPresenter());
    processHmiInputs.loop();
    delay(100);
    testanimate(); // Animate bitmaps
}
} // namespace main
