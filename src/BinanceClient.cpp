#include <iostream>
#include <thread>
#include <BinanceClient.hpp>
#include <OrderBook.hpp>
#include <OrderType.hpp>
#include <WSClient.hpp>
#include <json.hpp>

namespace arbitrage {
    using json = nlohmann::json;
    using namespace utils;
    void BinanceClient::connect(const std::string &assetA, const std::string &assetB)
    {
        _exchange.setName("Binance");
        std::thread connThread([this, &assetA, &assetB]() {
            std::cout << "Start a connection to Binance..." << std::endl;
            WSClient client;
            std::string subEndpoint = "/stream?streams=";
            std::string assetPair = assetA + assetB;
            toLowercase(assetPair);
            const std::string streamName1 = assetPair + "@depth5";
            toUppercase(assetPair);
            Connection &wss = client.connect("stream.binance.com", "9443", subEndpoint.append(streamName1));
            beast::multi_buffer buffer;

            try {
                while (1) {

                    std::cout << "Receiving data from Binance..."<< std::endl;
                    do {
                        wss.read_some(buffer, 512);
                    } while (!wss.is_message_done());

                    /*
                        The following code snippet is exchange-specific.
                    */
                    const std::string dataStr = beast::buffers_to_string(buffer.data());
                    json dataJ = json::parse(dataStr);
                    _exchange.updateMarket(createMarket(dataJ, assetPair));

                    buffer.consume(buffer.size());
                }
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        });
        connThread.detach();
    }

    Exchange &BinanceClient::getExchange()
    {
        return _exchange;
    }

    MarketPtr BinanceClient::createMarket(const json &dataJ, const std::string &assetPair)
    {
        MarketPtr market = std::make_shared<Market> ();

        OrderBookPtr orders = std::make_shared<OrderBook> ();

        for (const auto &bid: dataJ["data"]["bids"]) {
            std::string doubleStr = bid[0];
            orders->add(OrderType::Buy, std::stod(doubleStr), _exchange.getName());
        }

        for (const auto &ask: dataJ["data"]["asks"]) {
            std::string doubleStr = ask[0];
            orders->add(OrderType::Sell, std::stod(doubleStr), _exchange.getName());
        }

        market->insert(make_pair(assetPair, orders));
        return market;
    }
}
