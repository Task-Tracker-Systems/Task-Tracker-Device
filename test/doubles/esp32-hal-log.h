/**
 * @file
 * @brief Fake for testing on native
 */

#pragma once

template <typename... T>
inline void ESP_LOGD(const char *, const char *, T...)
{
}

template <typename... T>
inline void ESP_LOGV(const char *, const char *, T...)
{
}

template <typename... T>
inline void ESP_LOGI(const char *, const char *, T...)
{
}

template <typename... T>
inline void ESP_LOGW(const char *, const char *, T...)
{
}

template <typename... T>
void ESP_LOGE(const char *, const char *, T...)
{
}