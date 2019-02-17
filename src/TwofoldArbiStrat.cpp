#include <TwofoldArbiStrat.hpp>
#include <Exchange.hpp>
#include <Heap.hpp>
#include <Asset.hpp>
#include <vector>
#include <thread>
#include <memory>
namespace trading {
    namespace strategy {
        TwofoldArbiStrat::TwofoldArbiStrat(const std::string &assetA, const std::string &assetB)
        : _assetA(assetA), _assetB(assetB) {}

        void TwofoldArbiStrat::exec(std::vector<Exchange*> &exgs)
        {
            _stratLock.lock();
            _isOn = true;
            _stratLock.unlock();

            bool isOn = true;

            do {

                utils::MinHeap<AssetPtr> sells;
                utils::MaxHeap<AssetPtr> buys;

                std::vector<std::thread> thPool;

                std::clock_t timer_start = std::clock();

                for (int i = 0; i < exgs.size(); ++i) {
                    thPool.push_back(std::thread([&sells, &exgs, i, this] () {
                        sells.insert(exgs[i]->getSellAsset(_assetA, _assetB));
                    }));
                    thPool.push_back(std::thread([&buys, &exgs, i, this] () {
                        buys.insert(exgs[i]->getBuyAsset(_assetA, _assetB));
                    }));
                }

                for (auto &th: thPool) {
                    th.join();
                }

                std::clock_t timer_end = std::clock();


                AssetPtr buyAsset = sells.getTop();
                AssetPtr sellAsset = buys.getTop();

                double riskFactor = 0.9; // the amount that may not be present when the bot sells the asset

                double amtDiff = sellAsset->amt * riskFactor - buyAsset->amt;

                // The profitable amount has to be greater than the trading fees due; otherwise, it makes no sense to execute the trade.
                if ((sellAsset->val > buyAsset->val) && (amtDiff >= 0) && ((sellAsset->val * (1 - sellAsset->fee) - buyAsset->val * (1 + buyAsset->fee)) > 0)) {
                    std::cout << "==================== Arbitrage Opportunity (" << _assetA << "/" << _assetB << ") ====================" << std::endl;
                    std::cout <<
                        "Buy at " << buyAsset->id << ": " << buyAsset->val
                        << " - " <<
                        "Sell at " << sellAsset->id << ": " << sellAsset->val
                    << std::endl;
                    std::cout << "Amount: " << buyAsset->amt << std::endl;
                    std::cout << "Prices Access Duration: " << 1000000.0 * (timer_end - timer_start) / CLOCKS_PER_SEC << " ns" << std::endl;
                }

                _stratLock.lock();
                isOn = _isOn;
                _stratLock.unlock();
            } while (isOn);
        }

        void TwofoldArbiStrat::terminate()
        {
            _stratLock.lock();
            _isOn = false;
            _stratLock.unlock();
        }
    }
}
