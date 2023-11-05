/**
 * \file .
 */
#pragma once
#include <streambuf>

class SerialInputStreamBuffer : public std::streambuf
{
  public:
    SerialInputStreamBuffer();

  protected:
    int_type underflow() override;
    int sync() override;
};
