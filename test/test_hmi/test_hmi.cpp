#include <ArduinoFake.h>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <serial_interface/serial_port_interface.hpp>
#include <unity.h>
#include <user_interaction/IKeypad.hpp>
#include <user_interaction/Menu.hpp>
#include <user_interaction/Presenter.hpp>
#include <user_interaction/ProcessHmiInputs.hpp>
#include <user_interaction/display_factory_interface.hpp>
#include <user_interaction/keypad_factory_interface.hpp>
#include <user_interaction/statusindicators_factory_interface.hpp>
#include <utility>
#include <vector>

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
    std::vector<std::pair<std::uint8_t, void (*)(void)>> isr_collection;

    Fake(Method(ArduinoFake(), pinMode));
    When(Method(ArduinoFake(), attachInterrupt)).AlwaysDo([&isr_collection](const auto interruptNum, const auto userFunc, const auto mode) {
        isr_collection.emplace_back(interruptNum, userFunc);
        std::cout << "Added ISR for interrupt number " << static_cast<unsigned int>(interruptNum)
                  << " function pointer " << std::showbase << std::hex << reinterpret_cast<std::intptr_t>(userFunc) << std::resetiosflags(std::ios::showbase | std::ios::basefield) << std::endl;
    });

    static Menu singleMenu(board::getDisplay());
    static Presenter presenter(singleMenu, board::getStatusIndicators());
    ProcessHmiInputs processor(presenter, board::getKeypad());
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
