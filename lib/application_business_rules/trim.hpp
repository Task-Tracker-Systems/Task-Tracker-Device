#pragma once
#include <locale>

template <typename SequenceT>
SequenceT &trim(SequenceT &input, const std::locale &locale = std::locale())
{
    return input;
}