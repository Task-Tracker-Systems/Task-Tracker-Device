#pragma once

#include "serial_port.hpp"
#include <streambuf>

/**
 * Controls output of a character sequence to serial port.
 */
class SerialOutputStreamBuffer : public std::basic_streambuf<serial_port::CharType>
{
  private:
    char_type *const buffer_begin;
    char_type *const buffer_end;

  public:
    SerialOutputStreamBuffer(char_type *const buffer_begin, char_type *const buffer_end);

  protected:
    int_type overflow(const int_type input) override;
    int sync() override;

    /**
     * Writes the put area before the current character to serial interface.
     */
    void flush();

    /**
     * Sets the pointers to the extend of the buffer.
     */
    void resetOutputPointers();
};
