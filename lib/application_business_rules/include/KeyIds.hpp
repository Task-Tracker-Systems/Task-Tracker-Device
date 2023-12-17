#pragma once
#include <ostream>

enum class KeyId
{
    NONE,
    /*
   * may also be up
   */
    LEFT,
    /*
   * may also be down
   */
    RIGHT,
    ENTER,
    /*
   * may also be close, exit or ESC
   */
    BACK,
    TASK1,
    TASK2,
    TASK3,
    TASK4,
};

std::ostream &operator<<(std::ostream &os, const KeyId id);
