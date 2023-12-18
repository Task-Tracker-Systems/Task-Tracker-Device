#include "board.hpp"
#include "board_pins.hpp"
#include <Arduino.h>
#include <array>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <utility>

static board::HmiHandler callBack;

template <board::HmiSelection SELECTION>
static void isr()
{
    callBack(SELECTION);
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
static constexpr std::pair<PinType, board::HmiSelection> selectionForPins[] = {
    {board::button::pin::task1, board::HmiSelection::TASK1},
    {board::button::pin::task2, board::HmiSelection::TASK2},
    {board::button::pin::task3, board::HmiSelection::TASK3},
    {board::button::pin::task4, board::HmiSelection::TASK4},
    {board::button::pin::up, board::HmiSelection::UP},
    {board::button::pin::down, board::HmiSelection::DOWN},
    {board::button::pin::enter, board::HmiSelection::ENTER},
    {board::button::pin::back, board::HmiSelection::BACK},
};

void board::setup(const HmiHandler callbackFunction)
{
    // input pins
    callBack = callbackFunction;
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

    // output pins
    constexpr PinType outputPins[] = {
        board::led::pin::task1,
        board::led::pin::task2,
        board::led::pin::task3,
        board::led::pin::task4,
        board::buzzer::pin::on_off,
    };
    for (const auto outputPin : outputPins)
    {
        pinMode(outputPin, OUTPUT);
    }
}

void board::playTone(const unsigned int frequency, const std::chrono::milliseconds duration)
{
    tone(board::buzzer::pin::on_off, frequency, duration.count());
}

static PinType mapIndicatorIdToPin(const board::StatusIndicatorId id)
{
    switch (id)
    {
    case board::StatusIndicatorId::TASK1:
        return board::led::pin::task1;
        break;
    case board::StatusIndicatorId::TASK2:
        return board::led::pin::task2;
        break;
    case board::StatusIndicatorId::TASK3:
        return board::led::pin::task3;
        break;
    case board::StatusIndicatorId::TASK4:
        return board::led::pin::task4;
        break;
    default:
        throw std::runtime_error("no pin defined for selected status indicator");
        break;
    }
}

void board::setStatusIndicator(const StatusIndicatorId indicatorId, const bool toActive)
{
    const auto pin = mapIndicatorIdToPin(indicatorId);
    if (!toActive)
    {
        constexpr int minBrightness = 0;
        analogWrite(pin, minBrightness);
    }
    else
    {
        constexpr int maxBrightness = 255;
        constexpr int brightness = maxBrightness * 25 / 100.0;
        analogWrite(pin, brightness);
    }
}
