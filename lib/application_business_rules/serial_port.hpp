#include <array>
#include <functional>
#include <optional>
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
 * Reads a line from serial port.
 * 
 * Interprets the end of line as `\n`.
 * It will wait for data for the duration of the timeout.
 * \returns an empty string in case no data is read
 */
std::string readLine();

/**
 * Gets a line from serial port.
 * 
 * Interprets the end of line as `\n`.
 * \returns an object that does not contain a value in case no data is already available.
 */
std::optional<std::string> getLine();

/**
 * Callback which can handle strings.
 */
typedef std::function<void(const std::string &)> StringHandler;

/**
 * Set the handler to be called when a full line has been received via serial_port.
 * \param callback 
 */
void setCallbackToLineReception(const StringHandler &callback);

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
