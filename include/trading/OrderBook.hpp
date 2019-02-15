#pragma once

#include <vector>
#include <string>
#include <OrderType.hpp>
#include <Heap.hpp>
#include <Asset.hpp>

namespace trading {
    class OrderBook {
        public:
            AssetPtr getL(OrderType);
            AssetPtr getH(OrderType);
            void add(OrderType, double, double, const std::string &);
        private:
            utils::MinHeap<AssetPtr> _buyMinHeap;
            utils::MaxHeap<AssetPtr> _buyMaxHeap;
            utils::MinHeap<AssetPtr> _sellMinHeap;
            utils::MaxHeap<AssetPtr> _sellMaxHeap;
    };

    typedef std::shared_ptr<OrderBook> OrderBookPtr;
}
