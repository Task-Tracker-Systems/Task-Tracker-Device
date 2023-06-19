#include <array>
#include <ostream>

extern std::ostream &logging;

template <std::size_t BITS>
std::ostream &operator<<(std::ostream &os, const std::array<bool, BITS> &bitArray)
{
    os << "0b";
    for (const bool bit : bitArray)
    {
        if (bit)
        {
            os << "1";
        }
        else
        {
            os << "0";
        }
    }
    return os;
}
