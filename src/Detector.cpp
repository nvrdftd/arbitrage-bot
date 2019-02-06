#include <thread>
#include <string>
#include <Detector.hpp>
#include <Exchange.hpp>
#include <Strategy.hpp>

namespace arbitrage {

    void Detector::addExg(const std::string &name, Exchange *exchange)
    {
        stop();
        _strToExg.insert(make_pair(name, exchange));
    }

    void Detector::removeExg(const std::string &name)
    {
        stop();
        _strToExg.erase(name);
    }

    void Detector::addStrat(const std::string &name, Strategy *strat)
    {
        stop();
        _strToStrat.insert(make_pair(name, strat));
    }

    void Detector::removeStrat(const std::string &name)
    {
        stop();
        _strToStrat.erase(name);
    }

    void Detector::start()
    {

        /*
        /   The following makes sure that all the market data of the participating exchanges have been received
        /   before the detection starts.
        */

        auto exgIter = _strToExg.begin();
        while (exgIter != _strToExg.end()) {
            if (exgIter->second->exist()) {
                ++exgIter;
            } else {
                continue;
            }
        }

        std::vector<Exchange*> exgs;

        /*
            Collect exchange pointers into a vector. With the vector, execute a strategy.
        */
        for (auto &e: _strToExg) {
            exgs.push_back(e.second);
        }

        for (auto &e: _strToStrat) {
            e.second->exec(exgs);
        }

    }

    void Detector::stop()
    {
        for (auto &e: _strToStrat) {
            e.second->terminate();
        }
    }

}
