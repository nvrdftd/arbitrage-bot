#pragma once

#include <vector>
#include <mutex>
#include <Asset.hpp>

namespace utils {
    /*
        MinHeap & MaxHeap currently only support numeric types as template arguments;
    */
    using namespace trading;

    template <class A>
    struct is_asset_ptr {
        static const bool val = false;
    };

    template <>
    struct is_asset_ptr <AssetPtr> {
        static const bool val = true;
    };

    template <bool isMin, class A>
    class Heap {
        public:
            Heap()
            {
                static_assert(is_asset_ptr<A>::val, "'Asset' type needed");
            }
            A getTop()
            {
                _heapLock.lock();
                A data;
                if (_ptrHeap.size() == 0) {
                    return data;
                }
                data = _ptrHeap.at(0);
                _heapLock.unlock();
                return data;
            }
            void insert(A e)
            {
                static_assert(is_asset_ptr<A>::val, "'Asset' type needed");
                _heapLock.lock();
                _ptrHeap.push_back(e);
                bubbleUp(_ptrHeap.size() - 1);
                _heapLock.unlock();
            }
            void bubbleUp(int idx)
            {
                if (idx == 0) return;
                int nextIdx = (idx - 1) / 2;
                A &e1 = _ptrHeap.at(idx);
                A &e2 = _ptrHeap.at(nextIdx);
                bool toSwap = false;
                if (isMin) {
                    if (e1->val < e2->val) toSwap = true;
                } else {
                    if (e1->val > e2->val) toSwap = true;
                }
                if (toSwap) {
                    A _e1 = e1;
                    A _e2 = e2;
                    e1 = _e2;
                    e2 = _e1;
                    bubbleUp(nextIdx);
                }
            }
        private:
            std::mutex _heapLock;
            std::vector<A> _ptrHeap;
    };

    template <class A>
    using MinHeap = Heap<true, A>;

    template <class A>
    using MaxHeap = Heap<false, A>;

}
