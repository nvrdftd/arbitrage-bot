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

        // Assume that trading fee is always 0.1%, although some variaion of it may be applied to a specific account
        _exchange.setTradingFee(0.001);

        std::thread connThread([this, &assetA, &assetB]() {
            std::cout << "Start a connection to KuCoin..." << std::endl;
            RClient rClient;
            json resJ;

            std::string assetPair = assetA + "-" + assetB;
            toUppercase(assetPair);
            while (1) {
                resJ = rClient.get("openapi-v2.kucoin.com", "443", "/api/v1/market/orderbook/level2_20?symbol=" + assetPair);
                if (resJ["code"] == "200000") {
                    std::cout << "Receiving data from KuCoin..." << std::endl;
                    _exchange.updateMarket(createMarket(resJ, assetA + assetB));
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

        for (const auto &bid: data["bids"]) {
            const std::string priceStr = bid[0];
            const std::string amountStr = bid[1];
            orders->add(OrderType::Buy, std::stod(priceStr), std::stod(amountStr), _exchange);
        }

        for (const auto &ask: data["asks"]) {
            const std::string priceStr = ask[0];
            const std::string amountStr = ask[1];
            orders->add(OrderType::Sell, std::stod(priceStr), std::stod(amountStr), _exchange);
        }

        market->insert(make_pair(assetPair, orders));
        return market;
    }

}
