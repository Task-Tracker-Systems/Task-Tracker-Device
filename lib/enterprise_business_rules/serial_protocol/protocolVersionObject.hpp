//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     protocolVersionObject.hpp data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>
#include "helper.hpp"

namespace tasktrackersystems {
    /**
     * protocol version
     */

    using nlohmann::json;

    /**
     * protocol version
     */
    class protocolVersionObject {
        public:
        protocolVersionObject() :
            version_constraint(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::string("^(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.(0|[1-9]\\d*)(?:-([0-9A-Za-z-]+(?:\\.[0-9A-Za-z-]+)*))?(?:\\+([0-9A-Za-z-]+(?:\\.[0-9A-Za-z-]+)*))?$"))
        {}
        virtual ~protocolVersionObject() = default;

        private:
        std::string version;
        classMemberConstraints version_constraint;

        public:
        /**
         * Semantic Versioning (SemVer) string
         */
        const std::string & get_version() const { return version; }
        std::string & get_mutable_version() { return version; }
        void set_version(const std::string & value) { checkConstraint("version", version_constraint, value); this->version = value; }
    };
}
