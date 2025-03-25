/**
 * @file
 * @brief Fake for testing on native
 */

#pragma once

struct esp_err_t
{
};

inline void ESP_ERROR_CHECK(esp_err_t)
{
}
