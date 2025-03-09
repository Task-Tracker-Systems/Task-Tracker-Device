#pragma once
#include <cstddef>

struct Storage
{
    static void begin();
    static void end();
    static void test();
    static std::size_t size();
};
