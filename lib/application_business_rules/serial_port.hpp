#include <array>
#include <istream>
#include <ostream>

namespace serial_port
{
/**
 * Output stream for characters to serial port.
 * 
 * \pre call \ref initialize() before using
 */
extern std::ostream &cout;

extern std::istream &cin;

/**
 * Configures and initializes serial port.
 */
void initialize();
} // namespace serial_port

/**
 * Writes formatted bits to `ostream`.
 * 
 * \tparam BITS number of bits
 * \param os output stream to write to
 * \param bitArray the bits to output
 * \returns the stream
 */
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
