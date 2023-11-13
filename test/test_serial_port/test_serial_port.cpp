#include "serial_port.hpp"
#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

void setUp()
{
}

void tearDown()
{
}

void test_initialize()
{
    When(OverloadedMethod(ArduinoFake(Serial), begin, void(unsigned long))).AlwaysReturn();
    When(Method(ArduinoFake(Serial), flush)).AlwaysReturn();
    When(Method(ArduinoFake(), delay)).AlwaysReturn();
    serial_port::initialize();
    Verify(OverloadedMethod(ArduinoFake(Serial), begin, void(unsigned long))).AtLeastOnce();
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_initialize);

    UNITY_END();
}
