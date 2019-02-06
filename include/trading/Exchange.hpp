#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>
#include <boost/beast.hpp>
#include <OrderBook.hpp>


namespace trading {

    typedef std::shared_ptr<OrderBook> OrderBookPtr;
    typedef std::shared_ptr<std::unordered_map<std::string, OrderBookPtr> > MarketPtr;
    typedef std::unordered_map<std::string, OrderBookPtr> Market;

    namespace beast = boost::beast;
    class Exchange {
        public:
            double getBuyPrice(const std::string &, const std::string &);
            double getSellPrice(const std::string &, const std::string &);
            void updateMarket(MarketPtr);
            bool exist();
            void setName(const std::string &);
            const std::string &getName() const;
        private:
            std::string _name;
            MarketPtr _market;
            std::mutex _marketLock;
    };
}
