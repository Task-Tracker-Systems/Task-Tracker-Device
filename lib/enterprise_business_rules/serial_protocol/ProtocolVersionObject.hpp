//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     ProtocolVersionObject.hpp data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>
#include "helper.hpp"

namespace tasktrackersystems {
    /**
     * Protocol Version Object
     */

    using nlohmann::json;

    /**
     * Protocol Version Object
     */
    struct ProtocolVersionObject {
        int64_t major;
        int64_t minor;
        int64_t patch;
    };
}
