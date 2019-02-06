#include <OrderType.hpp>
#include <OrderBook.hpp>

namespace trading {

    double OrderBook::getL(OrderType type)
    {
        if (type == OrderType::Buy) {
            return _buyMinHeap.getMin().val;
        } else {
            return _sellMinHeap.getMin().val;
        }
    }
    double OrderBook::getH(OrderType type)
    {
        if (type == OrderType::Buy) {
            return _buyMaxHeap.getMax().val;
        } else {
            return _sellMaxHeap.getMax().val;
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
