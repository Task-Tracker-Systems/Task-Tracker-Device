#pragma once
#include "command_line_interpreter.hpp"

class ProtocolHandler
{
  public:
    typedef char CharType;
    static bool execute(const CharType *const commandLine);
};