#pragma once

#include <Strategy.hpp>
#include <Exchange.hpp>
#include <vector>
#include <string>

namespace trading {
    namespace strategy {
        class TwofoldArbiStrat: public Strategy {
            public:
                TwofoldArbiStrat(const std::string &, const std::string &);
                void exec(std::vector<Exchange*> &);
                void terminate();
            private:
                std::string _assetA;
                std::string _assetB;
        };
    }
}
