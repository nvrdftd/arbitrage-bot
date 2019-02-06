#pragma once

#include <unordered_map>
#include <string>
#include <Exchange.hpp>
#include <Strategy.hpp>

namespace arbitrage {

    using namespace trading;

    class Detector {
        public:
            void addExg(const std::string &, Exchange *);
            void removeExg(const std::string &);
            void addStrat(const std::string &, Strategy *);
            void removeStrat(const std::string &);
            void start();
            void stop();
        private:
            std::unordered_map<std::string, Exchange*> _strToExg;
            std::unordered_map<std::string, Strategy*> _strToStrat;
    };
}
