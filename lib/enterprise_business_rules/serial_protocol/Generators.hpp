//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Generators.hpp data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>
#include "helper.hpp"

#include "ProtocolVersionObject.hpp"

namespace tasktrackersystems {
    void from_json(const json & j, ProtocolVersionObject & x);
    void to_json(json & j, const ProtocolVersionObject & x);

    inline void from_json(const json & j, ProtocolVersionObject& x) {
        x.major = j.at("major").get<int64_t>();
        x.minor = j.at("minor").get<int64_t>();
        x.patch = j.at("patch").get<int64_t>();
    }

    inline void to_json(json & j, const ProtocolVersionObject & x) {
        j = json::object();
        j["major"] = x.major;
        j["minor"] = x.minor;
        j["patch"] = x.patch;
    }
}
