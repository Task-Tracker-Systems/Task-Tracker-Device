#include "main.hpp"
#include "ProcessHmiInputs.hpp"
#include "controller_factory_interface.hpp"
#include "logging.hpp"
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
    logging << std::endl
            << "begin program '" << programIdentificationString << "'" << std::endl;
}
void loop()
{
    constexpr unsigned long loopDurationMs = 250;
    static ProcessHmiInputs processHmiInputs(hmi::getController(), hmi::getPresenter());
    processHmiInputs.loop();
    delay(loopDurationMs);
}
} // namespace main
