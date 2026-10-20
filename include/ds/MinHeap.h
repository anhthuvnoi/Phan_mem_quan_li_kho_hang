#ifndef DS_MINHEAP_H
#define DS_MINHEAP_H

// Min-Heap — dùng cho: xuất kho FEFO (lô hết hạn sớm nhất), cảnh báo cận date
#include <stdexcept>
#include "ds/Compare.h"

template <typename T, typename Compare = DefaultLess<T>>
class MinHeap {
private:
    T* data;
    int capacity;
    int count;
    Compare cmp;

    void siftUp(int i);
    void siftDown(int i);
    void resize();
    void swapAt(int i, int j);

public:
    MinHeap(int cap = 16);
    MinHeap(const MinHeap& other);
    MinHeap& operator=(const MinHeap& other);
    ~MinHeap();

    void push(const T& value);
    void pop();                       
    T& top();                      
    const T& top() const;
    int size() const;
    bool empty() const;
    void clear();
};

#include "ds/MinHeap.cpp"

#endif // DS_MINHEAP_H
