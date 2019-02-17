#include <boost/beast.hpp>
#include <Exchange.hpp>
#include <OrderType.hpp>
#include <OrderBook.hpp>
#include <Asset.hpp>
#include <json.hpp>
#include <mutex>
#include <ctime>
#include <string>

namespace trading {
    using json = nlohmann::json;

    AssetPtr Exchange::getBuyAsset(const std::string &assetA, const std::string &assetB)
    {
        const std::string assetPair = assetA + assetB;

        _marketLock.lock();
        auto marketIter = _market->find(assetPair);
        AssetPtr asset = marketIter->second->getH(OrderType::Buy);
        _marketLock.unlock();

        return asset;
    }
    AssetPtr Exchange::getSellAsset(const std::string &assetA, const std::string &assetB)
    {
        const std::string assetPair = assetA + assetB;

        _marketLock.lock();
        auto marketIter = _market->find(assetPair);
        AssetPtr asset = marketIter->second->getL(OrderType::Sell);
        _marketLock.unlock();

        return asset;
    }
    void Exchange::updateMarket(MarketPtr market)
    {
        std::clock_t timer_start = std::clock();
        _marketLock.lock();
        _market = market;
        _marketLock.unlock();
        std::clock_t timer_end = std::clock();
        std::cout << "Market Swap Duration: " << 1000000.0 * (timer_end - timer_start) / CLOCKS_PER_SEC << " ns" << std::endl;
    }
    bool Exchange::exist()
    {
        return _market.use_count() > 0;
    }
    void Exchange::setName(const std::string &name)
    {
        _name = name;
    }
    const std::string &Exchange::getName() const
    {
        return _name;
    }
    void Exchange::setTradingFee(double tradingFee)
    {
        _tradingFee = tradingFee;
    }
    double Exchange::getTradingFee() const
    {
        return _tradingFee;
    }
}
