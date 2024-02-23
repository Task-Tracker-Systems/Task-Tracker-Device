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

#include "protocolVersionObject.hpp"

namespace tasktrackersystems {
    void from_json(const json & j, protocolVersionObject & x);
    void to_json(json & j, const protocolVersionObject & x);

    inline void from_json(const json & j, protocolVersionObject& x) {
        x.set_version(j.at("version").get<std::string>());
    }

    inline void to_json(json & j, const protocolVersionObject & x) {
        j = json::object();
        j["version"] = x.get_version();
    }
}
