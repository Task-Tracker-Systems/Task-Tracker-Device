#pragma once
#include <cstddef>
#include <memory>

namespace fs
{
class FS;
}

struct Storage
{
    static void begin();
    static void end();
    static std::shared_ptr<fs::FS> getFileSystem_locking();
    static std::size_t size();
};
