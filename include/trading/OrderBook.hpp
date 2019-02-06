#pragma once

#include <vector>
#include <string>
#include <OrderType.hpp>
#include <MinHeap.hpp>
#include <MaxHeap.hpp>
#include <Asset.hpp>

namespace trading {
    class OrderBook {
        public:
            double getL(OrderType);
            double getH(OrderType);
            void add(OrderType, double, const std::string &);
        private:
            utils::MinHeap<double, std::string, Asset<double, std::string> > _buyMinHeap;
            utils::MaxHeap<double, std::string, Asset<double, std::string> > _buyMaxHeap;
            utils::MinHeap<double, std::string, Asset<double, std::string> > _sellMinHeap;
            utils::MaxHeap<double, std::string, Asset<double, std::string> > _sellMaxHeap;
    };
}
