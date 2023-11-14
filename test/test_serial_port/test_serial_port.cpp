#include "serial_port.hpp"
#include <ArduinoFake.h>
#include <cstring>
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

void test_getLine()
{
    {
        // Test empty
        When(Method(ArduinoFake(Serial), available)).Return(0);
        const auto result = serial_port::getLine();
        TEST_ASSERT_FALSE(result.has_value());
    }

    {
        // Test non-empty
        constexpr auto testString = "Hello World";
        When(Method(ArduinoFake(Serial), available)).Return(std::strlen(testString) + 1);
        When(Method(ArduinoFake(Serial), readStringUntil)).Return(testString);
        const auto result = serial_port::getLine();
        TEST_ASSERT_EQUAL_STRING(testString, result.value_or("error").c_str());
    }
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_initialize);
    RUN_TEST(test_getLine);

    UNITY_END();
}
