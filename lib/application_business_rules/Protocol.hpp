#pragma once

class ProtocolHandler
{
  public:
    typedef char CharType;
    static bool execute(const CharType *const commandLine);
};
