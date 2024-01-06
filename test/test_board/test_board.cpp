#include "Keypad.hpp"
#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

void setUp()
{
}

void tearDown()
{
}

void test_Keypad()
{
    constexpr KeyId inputPinToKeyId[]{KeyId::BACK, KeyId::ENTER, KeyId::LEFT, KeyId::RIGHT, KeyId::TASK1, KeyId::TASK2, KeyId::TASK3, KeyId::TASK4};
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    Keypad keypad(inputPinToKeyId);

    // no key is pressed
    When(Method(ArduinoFake(), digitalRead)).AlwaysReturn(HIGH); // buttons are active low
    TEST_ASSERT_EQUAL(KeyId::NONE, keypad.getCurrentlyPressedKey());

    // second key is pressed
    When(Method(ArduinoFake(), digitalRead)).Return(HIGH, LOW, 6_Times(HIGH)); // buttons are active low
    TEST_ASSERT_EQUAL(KeyId::ENTER, keypad.getCurrentlyPressedKey());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_Keypad);

    UNITY_END();
}
