#pragma once
#include <Exchange.hpp>
#include <vector>

namespace trading {
    class Strategy {
        public:
            virtual void exec(std::vector<Exchange*> &) = 0;
            virtual void terminate() = 0;
        protected:
            std::mutex _stratLock;
            bool _isOn;
    };
}
