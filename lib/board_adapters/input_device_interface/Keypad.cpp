#include "Keypad.hpp"
#include <Arduino-wrapper.h>
#include <Worker.hpp>
#include <algorithm>
#include <array>
#include <board_pins.hpp>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <iterator>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>

static HmiHandler callBack;

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

template <board::PinType PIN>
static void reactOnPinChange()
{
    const bool isPressed = digitalRead(PIN) == LOW;
    if (isPressed)
    {
        const auto candidateKeyId = std::find_if(std::cbegin(selectionForPins), std::cend(selectionForPins), [](const auto pair) {
            return pair.first == PIN;
        });
        const bool foundKeyId = candidateKeyId != std::cend(selectionForPins);
        assert(foundKeyId);
        if (foundKeyId)
        {
            callBack(candidateKeyId->second);
        }
    }
}

template <board::PinType PIN>
static void isr()
{
    static Worker delayedStarter;
    // worker must be managed in a thread separate to the ISR to avoid deadlocks
    std::thread workerManagement([]() { delayedStarter.restart(
                                            reactOnPinChange<PIN>,
                                            std::chrono::milliseconds(200)); });
    workerManagement.detach();
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
        return {isr<VALUES[Is].first>...};
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
            CHANGE);
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
