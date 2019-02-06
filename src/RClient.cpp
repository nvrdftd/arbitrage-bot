#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <RClient.hpp>
#include <json.hpp>
#include <iostream>
#include <string>

namespace utils {

    namespace asio = boost::asio;
    namespace beast = boost::beast;

    using json = nlohmann::json;

    json RClient::get(const std::string &host, const std::string &port, const std::string &target) const
    {
        asio::io_context io;
        asio::ssl::context ssl_ctx(asio::ssl::context::sslv23);
        asio::ssl::stream<asio::ip::tcp::socket> stream(io, ssl_ctx);
        asio::ip::tcp::resolver resolver(io);
        const auto resolved = resolver.resolve(host, port);

        try {
            asio::connect(stream.next_layer(), resolved.begin(), resolved.end());

            stream.handshake(asio::ssl::stream_base::client);

            beast::http::request<beast::http::string_body> req(beast::http::verb::get, target, 11);

            req.set(beast::http::field::host, host);

            beast::http::write(stream, req);


            beast::multi_buffer buffer;

            beast::http::parser<false, beast::http::dynamic_body> parser;

            beast::error_code ec;

            do {
                beast::http::read_some(stream, buffer, parser, ec);
            } while (!parser.is_done());

            json a = json::parse(beast::buffers_to_string(parser.get().body().data()));

            return a;

        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

    }

}
