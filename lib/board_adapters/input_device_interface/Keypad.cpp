#include "Keypad.hpp"
#include <Arduino-wrapper.h>
#include <array>
#include <board_pins.hpp>
#include <cstddef>
#include <thread>
#include <type_traits>
#include <utility>

static HmiHandler callBack;

template <KeyId SELECTION>
static void isr()
{
    static std::thread *p_callbackThread = nullptr;
    const auto now = millis(); /* warning: `now()` from <chrono>/libc can not be used in ISRs */
    static std::remove_const_t<decltype(now)> lastCall;
    constexpr decltype(lastCall) debouncePeriod = 200; /* milliseconds */
    if (now - lastCall > debouncePeriod)
    {
        lastCall = now;
        if (p_callbackThread)
        {
            p_callbackThread->join();
            delete p_callbackThread;
        }
        p_callbackThread = new std::thread(callBack, SELECTION);
    }
}

/**
 * Generator for ISR function pointers.
 *
 * Uses an array to instantiate function templates and assembles an array of function pointers to those instances.
 * This is used to attach a value to the ISRs which can not accept any argument.
 * Instead the value is provided as non-type template argument to the ISR function template.
 *
 * @tparam T type of the array elements
 * @tparam N number of the array elements
 */
template <class T, std::size_t N>
struct FunctionPointerGenerator
{
    /**
     * Creates an array of function pointers to interrupt functions.
     *
     * Passing the values as template argument is necessary as they will be evaluated at compile time
     * for instantiating the function templates.
     *
     * @tparam VALUES array containing the values used for ISR function template instantiation
     * @returns an array with the same number of function pointers as the number of elements of the input values array
     */
    template <T (&VALUES)[N]>
    constexpr static auto createIsrPointers()
    {
        return createIsrPointers<VALUES>(std::make_index_sequence<N>());
    }

    /**
     * \copydoc createIsrPointers()
     * @tparam Is is a sequence of the indices to be used to access the array elements
     * @param indices is an object to derive `Is`
     */
    template <T (&VALUES)[N], std::size_t... Is>
    constexpr static std::array<void (*)(), N> createIsrPointers([[maybe_unused]] const std::index_sequence<Is...> indices)
    {
        return {isr<VALUES[Is].second>...};
    }
};

/**
 * Creates generator for function pointers.
 *
 * This function automatically deduces the template arguments for the generator.
 *
 * @param array of the same type and size to be used with the generator.
 *        Argument is used for template argument deduction only.
 * @tparam T the type of the array elements
 * @tparam N the number of array elements
 * @return generator which can be used to generate function pointers
 */
template <class T, std::size_t N>
static constexpr FunctionPointerGenerator<T, N> createFPG([[maybe_unused]] T (&array)[N])
{
    return FunctionPointerGenerator<T, N>();
}

/**
 * Maps HMI buttons to events.
 */
static constexpr std::pair<board::PinType, KeyId> selectionForPins[] = {
    {board::button::pin::task1, KeyId::TASK1},
    {board::button::pin::task2, KeyId::TASK2},
    {board::button::pin::task3, KeyId::TASK3},
    {board::button::pin::task4, KeyId::TASK4},
    {board::button::pin::up, KeyId::LEFT},
    {board::button::pin::down, KeyId::RIGHT},
    {board::button::pin::enter, KeyId::ENTER},
    {board::button::pin::back, KeyId::BACK},
};

Keypad::Keypad()
{
    // input pins
    constexpr auto functionPointers = createFPG(selectionForPins).createIsrPointers<selectionForPins>();
    std::size_t index = 0;
    for (const auto selectionForPin : selectionForPins)
    {
        pinMode(selectionForPin.first, INPUT_PULLUP);
        attachInterrupt(
            digitalPinToInterrupt(selectionForPin.first),
            functionPointers.at(index),
            FALLING);
        index++;
    }
}

void Keypad::setCallback(std::function<void(KeyId)> callbackFunction)
{
    callBack = callbackFunction;
}

bool Keypad::isKeyPressed(const KeyId keyInquiry)
{
    return false; //TODO
}
