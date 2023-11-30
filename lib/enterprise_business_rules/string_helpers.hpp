#pragma once
#include <cstring>
#include <cwchar>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * Compares two null-terminated byte strings lexicographically.
 * 
 * This is a wrapper to std::wcscmp() and std::strcmp() specialized for 
 * the character types.
 * 
 * \tparam CharType is the type of character to be used
 * \param lhs pointer to the null-terminated byte strings to compare
 * \param rhs pointer to the null-terminated byte strings to compare
 * \retval Negative value if lhs appears before rhs in lexicographical order.
 * \retval Zero if lhs and rhs compare equal.
 * \retval Positive value if lhs appears after rhs in lexicographical order.
 */
template <typename CharType>
int strcmp_g(const CharType *const lhs, const CharType *const rhs);

// Tokenize a string using std::quoted
/**
 * Tokenize a string while considering quoted sequences.
 * 
 * The input is analyzed and for each sub-sequence which is separated by whitespaces, a token is extracted.
 * With exception of sub-sequences which are quoted.
 * Those are extracted as a whole.
 * 
 * Quotes are according to `std::quoted`: '"' and escape characters `\`.
 * 
 * \tparam CharType is the type of character to be used (for example `char`)
 * \param input is the string to be analyzed
 * \throws std::runtime_error in case the extraction of a sub-sequence failed
 * \returns a set of strings
 */
template <typename CharType>
std::vector<std::basic_string<CharType>> tokenizeQuoted(const std::basic_string<CharType> &input)
{
    std::basic_istringstream<CharType> iss(input);
    std::vector<std::basic_string<CharType>> tokens;

    while (!(iss >> std::ws).eof())
    {
        std::basic_string<CharType> token;
        iss >> std::quoted(token);
        if (iss.fail())
        {
            throw std::runtime_error("failed to tokenize string: '" + input + "'");
        }
        tokens.push_back(token);
    }

    return tokens;
}