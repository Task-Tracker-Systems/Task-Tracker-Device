/**
 * \file .
 */
#pragma once
#include <streambuf>

class SerialInputStreamBuffer : public std::streambuf
{
  private:
    char_type *const buffer_begin;
    char_type *const buffer_end;

  public:
    SerialInputStreamBuffer(char_type *const buffer_begin, char_type *const buffer_end);

  protected:
    int_type underflow() override;
};
