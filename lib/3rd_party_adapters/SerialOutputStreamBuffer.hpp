#pragma once

#include <streambuf>

class SerialOutputStreamBuffer : public std::streambuf
{
  private:
    char_type *const buffer_begin;
    char_type *const buffer_end;

  public:
    SerialOutputStreamBuffer(char_type *const buffer_begin, char_type *const buffer_end);

  protected:
    int_type overflow(const int_type input) override;
    int sync() override;
    void flush();
    void resetOutputPointers();
};
