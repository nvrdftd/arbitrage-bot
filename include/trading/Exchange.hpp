#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>
#include <boost/beast.hpp>
#include <OrderBook.hpp>
#include <Asset.hpp>


namespace trading {

    class OrderBook;

    typedef std::shared_ptr<OrderBook> OrderBookPtr;
    typedef std::shared_ptr<std::unordered_map<std::string, OrderBookPtr> > MarketPtr;
    typedef std::unordered_map<std::string, OrderBookPtr> Market;

    namespace beast = boost::beast;
    class Exchange {
        public:
            AssetPtr getBuyAsset(const std::string &, const std::string &);
            AssetPtr getSellAsset(const std::string &, const std::string &);
            void updateMarket(MarketPtr);
            bool exist();
            void setName(const std::string &);
            const std::string &getName() const;
            void setTradingFee(double);
            double getTradingFee() const;
        private:
            std::string _name;
            MarketPtr _market;
            std::mutex _marketLock;
            double _tradingFee;
    };
}
