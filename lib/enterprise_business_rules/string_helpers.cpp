#include "string_helpers.hpp"

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
