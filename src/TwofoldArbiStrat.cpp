#include <TwofoldArbiStrat.hpp>
#include <Exchange.hpp>
#include <MinHeap.hpp>
#include <MaxHeap.hpp>
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

                utils::MinHeap<double, std::string, Asset<double, std::string> > sells;
                utils::MaxHeap<double, std::string, Asset<double, std::string> > buys;

                std::vector<std::thread> thPool;

                std::clock_t timer_start = std::clock();

                for (int i = 0; i < exgs.size(); ++i) {
                    thPool.push_back(std::thread([&sells, &exgs, i, this] () {
                        sells.insert(exgs[i]->getSellPrice(_assetA, _assetB), exgs[i]->getName());
                    }));
                    thPool.push_back(std::thread([&buys, &exgs, i, this] () {
                        buys.insert(exgs[i]->getBuyPrice(_assetA, _assetB), exgs[i]->getName());
                    }));
                }

                for (auto &th: thPool) {
                    th.join();
                }

                std::clock_t timer_end = std::clock();


                Asset<double, std::string> buyAsset = sells.getMin();
                Asset<double, std::string> sellAsset = buys.getMax();

                if (sellAsset.val > buyAsset.val) {
                    std::cout << "==================== Arbitrage Opportunity ====================" << std::endl;
                    std::cout <<
                        "Buy at " << buyAsset.id << ": " << buyAsset.val
                        << " - " <<
                        "Sell at " << sellAsset.id << ": " << sellAsset.val
                    << std::endl;
                    std::cout << "!!!!!!!!!! Detected !!!!!!!!!!" << std::endl;
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
