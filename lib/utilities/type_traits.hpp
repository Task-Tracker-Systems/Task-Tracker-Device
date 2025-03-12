#pragma once

#include <type_traits>

/**
 * \see std::to_underlying (since C++23)
 */
template <class Enum>
constexpr auto to_underlying(const Enum enumerator)
{
    return static_cast<std::underlying_type_t<Enum>>(enumerator);
}
