#include <OrderType.hpp>
#include <OrderBook.hpp>
#include <Asset.hpp>
#include <memory>

namespace trading {

    AssetPtr OrderBook::getL(OrderType type)
    {
        if (type == OrderType::Buy) {
            return _buyMinHeap.getTop();
        } else {
            return _sellMinHeap.getTop();
        }
    }
    AssetPtr OrderBook::getH(OrderType type)
    {
        if (type == OrderType::Buy) {
            return _buyMaxHeap.getTop();
        } else {
            return _sellMaxHeap.getTop();
        }
    }
    void OrderBook::add(OrderType type, double price, double amount, const std::string &name)
    {
        AssetPtr asset = std::make_shared<Asset<double, std::string> > ();
        asset->val = price;
        asset->amt = amount;
        asset->id = name;
        if (type == OrderType::Buy) {
            _buyMinHeap.insert(asset);
            _buyMaxHeap.insert(asset);
        } else {
            _sellMinHeap.insert(asset);
            _sellMaxHeap.insert(asset);
        }
    }
}
