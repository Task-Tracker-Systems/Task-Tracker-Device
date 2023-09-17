#pragma once

#include "IKeypad.hpp"
#include "board_config.hpp"
#include <cstddef>
#include <iterator>
#include <optional>

class Keypad : public IKeypad
{
  private:
    static constexpr board::PinType inputPins[] = {
        board::button::pin::task1,
        board::button::pin::task2,
        board::button::pin::task3,
        board::button::pin::task4,
        board::button::pin::up,
        board::button::pin::down,
        board::button::pin::enter,
        board::button::pin::back,
    };

  public:
    static constexpr auto numberOfPins = std::size(inputPins);
    virtual KeyId getCurrentlyPressedKey() const override;
    Keypad(const KeyId (&inputPinToKeyId)[numberOfPins]);

  private:
    const KeyId (&inputMapping)[numberOfPins];
    std::optional<std::size_t> getFirstPressedKeyIndex() const;
};
