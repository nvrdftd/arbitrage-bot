#include <iostream>
#include <thread>
#include <exception>
#include <KuCoinClient.hpp>
#include <OrderBook.hpp>
#include <OrderType.hpp>
#include <RClient.hpp>
#include <WSClient.hpp>
#include <json.hpp>

namespace arbitrage {

    using namespace utils;
    void KuCoinClient::connect(const std::string &assetA, const std::string &assetB)
    {
        _exchange.setName("KuCoin");
        std::thread connThread([this, &assetA, &assetB]() {
            std::cout << "Start a connection to KuCoin..." << std::endl;

            RClient rClient;
            WSClient wClient;

            json resJ = rClient.get("kitchen.kucoin.com", "443", "/v1/bullet/usercenter/loginUser?protocol=websocket&encrypt=true");
            const std::string bulletToken = resJ["data"]["bulletToken"];
            const auto servers = resJ["data"]["instanceServers"];
            if (servers.size() == 0) throw "KuCoin service down";
            const auto &server = servers[0];
            const std::string serverEndpoint = server["endpoint"];
            const std::string endpoint = serverEndpoint.substr(6);
            const std::string host = endpoint.substr(0, endpoint.find('/'));
            const std::string port = "443";
            const std::string target = endpoint.substr(endpoint.find('/'));
            Connection &wss = wClient.connect(host,  port, target + "?bulletToken=" + bulletToken + "&format=json&resource=api");

            /* Subscribe to one of the topics from the exchange */

            const std::string assetPair = assetA + "-" + assetB;
            const std::string topic = "/market/" + assetPair + "_TICK";

            json reqJ = {
                {"id", 1},
                {"type", "subscribe"},
                {"topic", topic},
                {"req", 1}
            };

            beast::multi_buffer buffer;

            beast::ostream(buffer) << reqJ.dump();

            std::clock_t timer_start = std::clock();
            std::clock_t timer_end = std::clock();

            try {
                while (1) {
                    beast::buffers_suffix<beast::multi_buffer::const_buffers_type> constBuffer(buffer.data());
                    if(asio::buffer_size(constBuffer) > 512) {

                        wss.write_some(false, beast::buffers_prefix(512, constBuffer));
                        constBuffer.consume(512);

                    } else {
                        wss.write_some(true, constBuffer);
                        timer_start = std::clock();
                        timer_end = std::clock();
                        reqJ = {
                            {"id", 1},
                            {"type", "ping"}
                        };
                        break;
                    }
                }
                buffer.consume(buffer.size());
                while (1) {
                    while (((timer_end - timer_start) / CLOCKS_PER_SEC) > 40) {

                        beast::ostream(buffer) << reqJ.dump();

                        beast::buffers_suffix<beast::multi_buffer::const_buffers_type> constBuffer(buffer.data());

                        if(asio::buffer_size(constBuffer) > 512) {

                            wss.write_some(false, beast::buffers_prefix(512, constBuffer));
                            constBuffer.consume(512);

                        } else {
                            wss.write_some(true, constBuffer);
                            timer_start = std::clock();
                            break;
                        }
                    }

                    buffer.consume(buffer.size());

                    std::cout << "Receiving data from KuCoin..." << std::endl;
                    do {
                        wss.read_some(buffer, 512);
                    } while (!wss.is_message_done());

                    const std::string dataStr = beast::buffers_to_string(buffer.data());
                    buffer.consume(buffer.size());
                    json dataJ = json::parse(dataStr);
                    if (dataJ.size() < 4) continue;
                    _exchange.updateMarket(createMarket(dataJ, assetA + assetB));
                    timer_end = std::clock();

                }
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
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

        orders->add(OrderType::Buy, data["buy"], _exchange.getName());
        orders->add(OrderType::Sell, data["sell"], _exchange.getName());

        market->insert(make_pair(assetPair, orders));
        return market;
    }

}
