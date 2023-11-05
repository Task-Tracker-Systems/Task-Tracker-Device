#include <array>
#include <ostream>

namespace serial_port
{
extern std::ostream &cout;
void initialize();
} // namespace serial_port

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
