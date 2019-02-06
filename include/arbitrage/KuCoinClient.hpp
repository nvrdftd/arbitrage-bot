#pragma once

#include <WSClient.hpp>
#include <Exchange.hpp>
#include <json.hpp>
#include <string>

namespace arbitrage {
    using namespace trading;
    using json = nlohmann::json;

    class KuCoinClient {
        public:
            void connect(const std::string &, const std::string &);
            Exchange &getExchange();
            MarketPtr createMarket(const json &, const std::string &);
        private:
            Exchange _exchange;
    };
}
