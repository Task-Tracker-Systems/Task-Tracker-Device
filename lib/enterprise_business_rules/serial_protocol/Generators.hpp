#pragma once
#include "ProtocolVersionObject.hpp"
#include <nlohmann/json.hpp>

namespace task_tracker_systems
{
inline void to_json(nlohmann::json &j, const ProtocolVersionObject &x)
{
    j = nlohmann::json::object();
    j["major"] = x.major;
    j["minor"] = x.minor;
    j["patch"] = x.patch;
}
} // namespace task_tracker_systems
