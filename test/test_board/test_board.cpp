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
    Mock<Keypad::InputShiftRegister> isr;
    constexpr Keypad::InputMapping inputPinToKeyId{KeyId::BACK, KeyId::ENTER, KeyId::LEFT, KeyId::RIGHT, KeyId::TASK1, KeyId::TASK2, KeyId::TASK3, KeyId::TASK4};
    Keypad keypad(isr.get(), inputPinToKeyId);

    // First no key is pressed
    When(Method(isr, readRegister)).Return({0});
    TEST_ASSERT_EQUAL(KeyId::NONE, keypad.getCurrentlyPressedKey());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_Keypad);

    UNITY_END();
}
