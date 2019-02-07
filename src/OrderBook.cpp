#include <OrderType.hpp>
#include <OrderBook.hpp>

namespace trading {

    double OrderBook::getL(OrderType type)
    {
        if (type == OrderType::Buy) {
            return _buyMinHeap.getTop().val;
        } else {
            return _sellMinHeap.getTop().val;
        }
    }
    double OrderBook::getH(OrderType type)
    {
        if (type == OrderType::Buy) {
            return _buyMaxHeap.getTop().val;
        } else {
            return _sellMaxHeap.getTop().val;
        }
    }
    void OrderBook::add(OrderType type, double price, const std::string &name)
    {
        if (type == OrderType::Buy) {
            _buyMinHeap.insert(price, name);
            _buyMaxHeap.insert(price, name);
        } else {
            _sellMinHeap.insert(price, name);
            _sellMaxHeap.insert(price, name);
        }
    }
}
