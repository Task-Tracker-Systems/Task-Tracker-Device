#include <ArduinoFake.h>
#include <serial_interface/serial_port_interface.hpp>
#include <unity.h>
#include <user_interaction/IKeypad.hpp>
#include <user_interaction/Menu.hpp>
#include <user_interaction/Presenter.hpp>
#include <user_interaction/ProcessHmiInputs.hpp>
#include <user_interaction/display_factory_interface.hpp>
#include <user_interaction/statusindicators_factory_interface.hpp>

using namespace fakeit;

// mocks

namespace board
{
IDisplay &getDisplay()
{
    Mock<IDisplay> fakeDisplay;
    return fakeDisplay.get();
}
} // namespace board

namespace serial_port
{
std::basic_ostream<CharType> &cout = std::cout;
}

void setUp()
{
}

void tearDown()
{
}

void test_Controller()
{
    Mock<IKeypad> fakeKeypad;
    static Menu singleMenu(board::getDisplay());
    static Presenter presenter(singleMenu, board::getStatusIndicators());
    ProcessHmiInputs processor(presenter, fakeKeypad.get());
    /*
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
    */
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_Controller);

    UNITY_END();
}
