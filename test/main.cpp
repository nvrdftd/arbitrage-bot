#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <BinanceClient.hpp>
#include <KuCoinClient.hpp>
#include <Detector.hpp>
#include <TwofoldArbiStrat.hpp>

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "Two types of assets needed" << std::endl;
        return 0;
    }

    arbitrage::BinanceClient bClient;
    arbitrage::KuCoinClient kClient;

    const std::string assetA(argv[1]);
    const std::string assetB(argv[2]);

    bClient.connect(assetA, assetB);
    kClient.connect(assetA, assetB);

    trading::Exchange &exchangeOne = bClient.getExchange();
    trading::Exchange &exchangeTwo = kClient.getExchange();

    /*
    /   Strategy 1:
    /   We shall get the prices of the same coin pair from two distinct exchanges in parallel.
    /   We can either look at the buy price at one exchange and the sell price at another,
    /   or the other way around, for the same coin pair.
    /
    /   Strategy 2:
    /   Future work
    */

    trading::strategy::TwofoldArbiStrat stratOne(assetA, assetB);

    arbitrage::Detector arbitrageur;

    arbitrageur.addStrat("Twofold Arbitrage", &stratOne);
    arbitrageur.addExg("Binance", &exchangeOne);
    arbitrageur.addExg("KuCoin", &exchangeTwo);

    std::thread newth([&] () {
        arbitrageur.start();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(3600000));
    arbitrageur.stop();

    newth.join();

    return 0;
}
