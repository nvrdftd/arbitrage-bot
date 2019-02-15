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
            /*
            *   The following Heaps are used to maintain the min or max of any OrderType for a specific asset pair
            *   since the JSON data received from online exchanges may not be in the expected order.
            *   Thus, it always ensures to gain as many profits as possible.
            *   However, _buyMinHeap and _sellMaxHeap would not be useful in this case.
            */
            utils::MinHeap<AssetPtr> _buyMinHeap;
            utils::MaxHeap<AssetPtr> _buyMaxHeap;
            utils::MinHeap<AssetPtr> _sellMinHeap;
            utils::MaxHeap<AssetPtr> _sellMaxHeap;
    };

    typedef std::shared_ptr<OrderBook> OrderBookPtr;
}
