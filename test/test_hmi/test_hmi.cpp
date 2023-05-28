#include "Controller.hpp"
#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

void setUp()
{
}

void tearDown()
{
}

void test_Controller()
{
    Mock<IKeypad> fakeKeypad;
    Controller controller(fakeKeypad.get());

    {
        // First no key is pressed
        When(Method(fakeKeypad, getCurrentlyPressedKey)).Return(KeyId::NONE);
        const auto event_cand = controller.checkHmiInput();
        TEST_ASSERT_EQUAL(false, event_cand.has_value());
    }

    {
        // Again no key is pressed
        When(Method(fakeKeypad, getCurrentlyPressedKey)).Return(KeyId::NONE);
        const auto event_cand = controller.checkHmiInput();
        TEST_ASSERT_EQUAL(false, event_cand.has_value());
    }

    {
        // Now some key is pressed
        When(Method(fakeKeypad, getCurrentlyPressedKey)).Return(KeyId::ENTER);
        const auto event_cand = controller.checkHmiInput();
        TEST_ASSERT_EQUAL(false, event_cand.has_value());
    }

    {
        // That key is not released yet
        When(Method(fakeKeypad, getCurrentlyPressedKey)).Return(KeyId::ENTER);
        const auto event_cand = controller.checkHmiInput();
        TEST_ASSERT_EQUAL(false, event_cand.has_value());
    }

    {
        // That key is not released yet
        When(Method(fakeKeypad, getCurrentlyPressedKey)).Return(KeyId::NONE);
        const auto event_cand = controller.checkHmiInput();
        TEST_ASSERT_EQUAL(KeyId::ENTER, event_cand.value());
    }
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_Controller);

    UNITY_END();
}
