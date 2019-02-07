#pragma once

#include <vector>
#include <mutex>

namespace utils {
    /*
        MinHeap & MaxHeap currently only support numeric types as template arguments;
    */

    template <bool isMin, class T, class X, class A>
    class Heap {
        public:
            A getTop()
            {
                _heapLock.lock();
                A data;
                if (_valHeap.size() == 0) {
                    return data;
                }
                data.val = _valHeap.at(0);
                data.id = _nameHeap.at(0);
                _heapLock.unlock();
                return data;
            }
            void insert(const T &e, const X &n)
            {
                _heapLock.lock();
                _valHeap.push_back(e);
                _nameHeap.push_back(n);
                bubbleUp(_valHeap.size() - 1);
                _heapLock.unlock();
            }
            void bubbleUp(int idx)
            {
                if (idx == 0) return;
                int nextIdx = (idx - 1) / 2;
                T &e1 = _valHeap.at(idx);
                T &e2 = _valHeap.at(nextIdx);
                X &n1 = _nameHeap.at(idx);
                X &n2 = _nameHeap.at(nextIdx);
                if (isMin) {
                    if (e1 < e2) {
                        T _e1 = e1;
                        T _e2 = e2;
                        X _n1 = n1;
                        X _n2 = n2;
                        e1 = _e2;
                        e2 = _e1;
                        n1 = _n2;
                        n2 = _n1;
                        bubbleUp(nextIdx);
                    }
                } else {
                    if (e1 > e2) {
                        T _e1 = e1;
                        T _e2 = e2;
                        X _n1 = n1;
                        X _n2 = n2;
                        e1 = _e2;
                        e2 = _e1;
                        n1 = _n2;
                        n2 = _n1;
                        bubbleUp(nextIdx);
                    }
                }
            }
        private:
            std::mutex _heapLock;
            std::vector<T> _valHeap;
            std::vector<X> _nameHeap;
    };

    template<class T, class X, class A>
    using MinHeap = Heap<true, T, X, A>;

    template<class T, class X, class A>
    using MaxHeap = Heap<false, T, X, A>;

}
