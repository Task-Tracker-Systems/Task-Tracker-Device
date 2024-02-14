#include <Arduino-wrapper.h>
#include <algorithm>
#include <board_pins.hpp>
#include <board_types.hpp>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <map>
#include <serial_interface/serial_port.hpp>
#include <tasks/Task.hpp>
#include <thread>
#include <unity.h>
#include <user_interaction/IKeypad.hpp>
#include <user_interaction/Menu.hpp>
#include <user_interaction/Presenter.hpp>
#include <user_interaction/ProcessHmiInputs.hpp>
#include <user_interaction/display_factory_interface.hpp>
#include <user_interaction/keypad_factory_interface.hpp>
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
    struct IsrHandle
    {
        board::PinType associatedPin;
        void (*isrFunction)(void);
    };

    std::map<board::PinType, void (*)(void)> isr_collection;

    Fake(Method(ArduinoFake(), pinMode));
    Fake(Method(ArduinoFake(), analogWrite));
    Fake(Method(ArduinoFake(), tone));
    When(Method(ArduinoFake(), attachInterrupt)).AlwaysDo([&isr_collection](const auto interruptNum, const auto userFunc, const auto mode) {
        isr_collection.emplace(interruptNum, userFunc);
        std::cout << "Added ISR for interrupt number " << static_cast<unsigned int>(interruptNum)
                  << " function pointer " << std::showbase << std::hex << reinterpret_cast<std::intptr_t>(userFunc) << std::resetiosflags(std::ios::showbase | std::ios::basefield) << std::endl;
    });

    Menu singleMenu(board::getDisplay());
    Presenter presenter(singleMenu, board::getStatusIndicators());
    ProcessHmiInputs processor(presenter, board::getKeypad());

    auto &task1 = std::begin(device::tasks)->second;
    const auto isrTask1 = isr_collection.find(board::button::pin::task1);
    TEST_ASSERT_NOT_EQUAL(std::end(isr_collection), isrTask1);
    When(Method(ArduinoFake(), millis)).Return(700); // assume the processor is already running for some time
    isrTask1->second();                              // start task
    while (!task1.isRunning())
    {
        // TODO it would be better to explicitly check for the "stop" task to be finished
        std::this_thread::yield(); // give the task handler time to finish before the test interferes
    }
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1000ms);
    When(Method(ArduinoFake(), millis)).Return(700 + 1337); // used in ISR for debouncing
    isrTask1->second();                                     // stop task
    while (task1.isRunning())
    {
        // TODO it would be better to explicitly check for the "start" task to be finished
        std::this_thread::yield(); // give the task handler time to finish before the test interferes
    }
    const auto measured = std::chrono::duration_cast<std::chrono::milliseconds>(task1.getRecordedDuration());
    TEST_ASSERT_INT_WITHIN(10, 1000, measured.count());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_Controller);

    UNITY_END();
}
