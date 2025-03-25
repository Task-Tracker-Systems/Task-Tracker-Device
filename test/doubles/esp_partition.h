/**
 * @file
 * @brief Fake for testing on native
 */

#pragma once
#include "esp_err.h"
#include <stddef.h>

struct esp_partition_t;

inline esp_err_t esp_partition_erase_range(const esp_partition_t *, size_t, size_t)
{
    return {};
}

inline esp_err_t esp_partition_read(const esp_partition_t *, size_t, void *, size_t)
{
    return {};
}

inline esp_err_t esp_partition_write(const esp_partition_t *, size_t, const void *, size_t)
{
    return {};
}

constexpr char FFAT_PARTITION_LABEL[] = "FFAT_PARTITION_LABEL";