#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <iostream>
#include <string>
#include <WSClient.hpp>

namespace utils {

    WSClient::WSClient(): _ssl_ctx(asio::ssl::context::sslv23), _wss(_io, _ssl_ctx) {}

    Connection &WSClient::connect(const std::string &host, const std::string &port, const std::string &target)
    {
        asio::ip::tcp::resolver resolver(_io);
        const auto resolved = resolver.resolve(host, port);

        try {
            asio::connect(_wss.next_layer().next_layer(), resolved.begin(), resolved.end());
            _wss.next_layer().handshake(asio::ssl::stream_base::client);
            _wss.handshake(host, target);
            return _wss;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;

        }


    }

    void WSClient::disconnect()
    {

    }
}
