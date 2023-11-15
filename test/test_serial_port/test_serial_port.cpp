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
        constexpr const serial_port::CharType *const testString = "Hello World";
        When(Method(ArduinoFake(Serial), available)).Return(std::strlen(testString) + 1);
        When(Method(ArduinoFake(Serial), readStringUntil)).Return(testString);
        const auto result = serial_port::getLine();
        TEST_ASSERT_EQUAL_STRING(testString, result.value_or("error").c_str());
    }
}

static serial_port::String receivedLine = "foo";

static void lineHandler(const serial_port::String &line)
{
    receivedLine = line;
}

// we know that this function is used to insert data from serial port
extern void serialEvent();

void test_subscribeToLine()
{
    serial_port::setCallbackForLineReception(lineHandler);

    constexpr const serial_port::CharType *const testLine = "C++\n";
    for (std::size_t i = 0; i < std::strlen(testLine); ++i)
    {
        When(Method(ArduinoFake(Serial), available)).Return(1, 0); // every time only 1 char is available
        When(Method(ArduinoFake(Serial), read)).Return(testLine[i]);
        serialEvent();
    }
    TEST_ASSERT_EQUAL_STRING(testLine, receivedLine.c_str());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_initialize);
    RUN_TEST(test_getLine);
    RUN_TEST(test_subscribeToLine);

    UNITY_END();
}
