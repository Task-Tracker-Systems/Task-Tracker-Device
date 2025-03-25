/**
 * @file
 * @brief Fake for testing on native
 */

#pragma once
#include "esp_partition.h"
#include <stddef.h>
#include <stdint.h>

namespace fs
{
struct FS
{
    bool begin(bool formatOnFail = false, const char *basePath = "/ffat", uint8_t maxOpenFiles = 10, const char *partitionLabel = (char *)FFAT_PARTITION_LABEL)
    {
        return true;
    }
    bool format(bool full_wipe = true, char *partitionLabel = (char *)FFAT_PARTITION_LABEL)
    {
        return true;
    }
    size_t totalBytes()
    {
        return 42;
    }
    size_t usedBytes()
    {
        return 42 / 2;
    }
    size_t freeBytes()
    {
        return totalBytes() - usedBytes();
    }
    void end()
    {
    }
    bool exists(const char *)
    {
        return true;
    }
};
} // namespace fs

extern fs::FS FFat;
