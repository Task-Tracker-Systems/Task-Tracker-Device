#include "main.hpp"
#include "ProcessHmiInputs.hpp"
#include "controller_factory_interface.hpp"
#include "presenter_factory_interface.hpp"
#include <Arduino.h>

namespace main
{
void setup(char const *programIdentificationString)
{
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
}
} // namespace main
