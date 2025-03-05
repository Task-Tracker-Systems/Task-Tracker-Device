#pragma once

struct Storage
{
    static void begin();
    static void waitForFileSystem();
};
