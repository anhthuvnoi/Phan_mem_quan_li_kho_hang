#ifndef DS_MAXHEAP_H
#define DS_MAXHEAP_H

// Max-Heap — dùng cho: gợi ý kho nguồn điều chuyển (kho có tồn cao nhất)
#include <stdexcept>
#include "ds/Compare.h"

template <typename T, typename Compare = DefaultLess<T>>
class MaxHeap {
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
    MaxHeap(int cap = 16);
    MaxHeap(const MaxHeap& other);
    MaxHeap& operator=(const MaxHeap& other);
    ~MaxHeap();

    void push(const T& value);
    void pop();                       
    T& top();                         
    const T& top() const;
    int size() const;
    bool empty() const;
    void clear();
};

#include "ds/MaxHeap.cpp"

#endif // DS_MAXHEAP_H
