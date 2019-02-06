#pragma once

#include <string>
#include <json.hpp>

namespace utils {

    using json = nlohmann::json;

    class RClient {
        public:
            json get(const std::string &, const std::string &, const std::string &) const;
    };

}
