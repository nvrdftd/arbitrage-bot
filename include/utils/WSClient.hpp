#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <string>

namespace utils {

    namespace asio = boost::asio;
    namespace beast = boost::beast;

    typedef beast::websocket::stream<asio::ssl::stream<asio::ip::tcp::socket> > Connection;

    class WSClient {

        public:
            WSClient();
            Connection &connect(const std::string &, const std::string &, const std::string &);
            void disconnect();
        private:
            asio::io_context _io;
            asio::ssl::context _ssl_ctx;
            Connection _wss;
    };
}
