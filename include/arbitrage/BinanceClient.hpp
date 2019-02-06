#pragma once

#include <OrderBook.hpp>
#include <Exchange.hpp>
#include <memory>
#include <json.hpp>
#include <cctype>
#include <string>

inline
void toUppercase(std::string &str)
{
    for (auto &c: str) {
        c = toupper(c);
    }
}

inline
void toLowercase(std::string &str)
{
    for (auto &c: str) {
        c = tolower(c);
    }
}

namespace arbitrage {
    using namespace trading;
    using json = nlohmann::json;

    class BinanceClient {
        public:
            void connect(const std::string &, const std::string &);
            Exchange &getExchange();
            MarketPtr createMarket(const json &, const std::string &);
        private:
            Exchange _exchange;
    };
}
