#pragma once
#include <cstring>
#include <cwchar>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename CharType>
int strcmp_g(const CharType *const lhs, const CharType *const rhs);

template <>
int strcmp_g(const char *const lhs, const char *const rhs)
{
    return std::strcmp(lhs, rhs);
}

template <>
int strcmp_g(const wchar_t *const lhs, const wchar_t *const rhs)
{
    return std::wcscmp(lhs, rhs);
}

// Tokenize a string using std::quoted
template <typename CharType>
std::vector<std::basic_string<CharType>> tokenizeQuoted(const std::basic_string<CharType> &input)
{
    std::basic_istringstream<CharType> iss(input);
    std::vector<std::basic_string<CharType>> tokens;

    std::basic_string<CharType> token;
    while (!iss.eof())
    {
        iss >> std::quoted(token);
        if (iss.fail())
        {
            throw std::runtime_error("failed to tokenize string: '" + input + "'");
        }
        tokens.push_back(token);
    }

    return tokens;
}