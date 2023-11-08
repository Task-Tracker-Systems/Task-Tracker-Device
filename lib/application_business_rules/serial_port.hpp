#include <array>
#include <ostream>
#include <string>

namespace serial_port
{
/**
 * Output stream for characters to serial port.
 * 
 * \pre call \ref initialize() before using
 */
extern std::ostream &cout;

/**
 * Configures and initializes serial port.
 */
void initialize();

/**
 * Gets a null terminated string from serial interface.
 * 
 * If no character is available at the moment of the call,
 * it will be waited until the configured timeout occurs.
 */
std::string getString();
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
