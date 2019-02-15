#include <iostream>
#include <thread>
#include <exception>
#include <KuCoinClient.hpp>
#include <OrderBook.hpp>
#include <OrderType.hpp>
#include <RClient.hpp>
#include <WSClient.hpp>
#include <UtilLib.hpp>
#include <json.hpp>

namespace arbitrage {

    using namespace utils;
    void KuCoinClient::connect(const std::string &assetA, const std::string &assetB)
    {
        _exchange.setName("KuCoin");
        _exchange.setTradingFee(0.001);
        std::thread connThread([this, &assetA, &assetB]() {
            std::cout << "Start a connection to KuCoin..." << std::endl;
            RClient rClient;
            json resJ;

            std::string assetPair = assetA + "-" + assetB;
            toUppercase(assetPair);
            while (1) {
                resJ = rClient.get("api.kucoin.com", "443", "/v1/open/orders?symbol=" + assetPair);
                if (resJ["success"] == true) {
                    std::cout << "Receiving data from KuCoin..." << std::endl;
                    _exchange.updateMarket(createMarket(resJ, assetA + assetB));
                    continue;
                } else {
                    throw "KuCoin symbol error";
                }
            }
        });
        connThread.detach();
    }

    Exchange &KuCoinClient::getExchange()
    {
        return _exchange;
    }

    MarketPtr KuCoinClient::createMarket(const json &dataJ, const std::string &assetPair)
    {
        MarketPtr market = std::make_shared<Market> ();
        OrderBookPtr orders = std::make_shared<OrderBook> ();

        const auto &data = dataJ["data"];

        for (const auto &buy: data["BUY"]) {
            orders->add(OrderType::Buy, buy[0], buy[1], _exchange.getName());
        }

        for (const auto &sell: data["SELL"]) {
            orders->add(OrderType::Sell, sell[0], sell[1], _exchange.getName());
        }

        market->insert(make_pair(assetPair, orders));
        return market;
    }

}
