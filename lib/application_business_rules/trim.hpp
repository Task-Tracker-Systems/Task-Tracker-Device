#pragma once
#include <iterator>
#include <locale>
#include <utility>

template <typename SequenceT>
SequenceT ltrim(SequenceT &&input, const std::locale &locale = std::locale())
{
    const auto begin_trimmed = std::find_if(
        std::begin(input),
        std::end(input),
        [&](const auto ch) { return !std::isspace(ch, locale); });
    return SequenceT(begin_trimmed, std::end(input));
}

template <typename SequenceT>
SequenceT &ltrim(SequenceT &input, const std::locale &locale = std::locale())
{
    input = ltrim(std::move(input));
    return input;
}

template <typename SequenceT>
SequenceT rtrim(SequenceT &&input, const std::locale &locale = std::locale())
{
    const auto end_trimmed = std::find_if(
        std::rbegin(input),
        std::rend(input),
        [&](const auto ch) { return !std::isspace(ch, locale); });
    return SequenceT(std::begin(input), end_trimmed.base());
}

template <typename SequenceT>
SequenceT &rtrim(SequenceT &input, const std::locale &locale = std::locale())
{
    input = rtrim(std::move(input));
    return input;
}

template <typename SequenceT>
SequenceT trim(SequenceT &&input, const std::locale &locale = std::locale())
{
    return rtrim(ltrim(std::move(input)));
}

template <typename SequenceT>
SequenceT &trim(SequenceT &input, const std::locale &locale = std::locale())
{
    return rtrim(ltrim(input));
}
