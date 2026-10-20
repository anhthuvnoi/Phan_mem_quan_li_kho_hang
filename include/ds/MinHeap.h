#ifndef DS_MINHEAP_H
#define DS_MINHEAP_H

#include <stdexcept> // thư viện xử lí ngoại lệ
#include "ds/Compare.h"

// cho phép min heap lưu trữ bất kì kiểu dữ liệu nào T
template <typename T, typename Compare = DefaultLess<T>>
class MinHeap {
private:
    T* data; // con trỏ quản lí mảng động chứa các phần tử của heap
    int capacity;
    int count;
    Compare cmp;

    void siftUp(int i); // vun lên - vun xuống
    void siftDown(int i);
    void resize();
    void swapAt(int i, int j); // đổi chỗ hai phần tử trong mảng

public:
    MinHeap(int cap = 16); // khởi tạo với dung lượng mặc định là 16
    MinHeap(const MinHeap& other); // copy constructor
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
