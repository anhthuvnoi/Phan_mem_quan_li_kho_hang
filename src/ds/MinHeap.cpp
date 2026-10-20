#ifndef DS_MINHEAP_CPP
#define DS_MINHEAP_CPP

#include "ds/MinHeap.h"

template <typename T, typename Compare>
MinHeap<T, Compare>::MinHeap(int cap)
    : data(nullptr), capacity(cap > 0 ? cap : 1), count(0), cmp() {
    data = new T[capacity];
}

template <typename T, typename Compare>
MinHeap<T, Compare>::MinHeap(const MinHeap& other)
    : data(new T[other.capacity]), capacity(other.capacity), count(other.count), cmp(other.cmp) {
    for (int i = 0; i < count; i++) data[i] = other.data[i];
}

template <typename T, typename Compare>
MinHeap<T, Compare>& MinHeap<T, Compare>::operator=(const MinHeap& other) {
    if (this != &other) {
        T* newData = new T[other.capacity];
        for (int i = 0; i < other.count; i++) newData[i] = other.data[i];
        delete[] data;
        data = newData;
        capacity = other.capacity;
        count = other.count;
        cmp = other.cmp;
    }
    return *this;
}

template <typename T, typename Compare>
MinHeap<T, Compare>::~MinHeap() {
    delete[] data;
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::swapAt(int i, int j) {
    T tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::resize() {
    int newCap = capacity * 2;
    T* newData = new T[newCap];
    for (int i = 0; i < count; i++) newData[i] = data[i];
    delete[] data;
    data = newData;
    capacity = newCap;
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (cmp(data[i], data[parent])) {     // con nhỏ hơn cha -> đổi chỗ
            swapAt(i, parent);
            i = parent;
        } else {
            break;
        }
    }
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::siftDown(int i) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < count && cmp(data[left], data[smallest])) smallest = left;
        if (right < count && cmp(data[right], data[smallest])) smallest = right;
        if (smallest == i) break;
        swapAt(i, smallest);
        i = smallest;
    }
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::push(const T& value) {
    if (count == capacity) resize();
    data[count] = value;
    siftUp(count);
    count++;
}

template <typename T, typename Compare>
void MinHeap<T, Compare>::pop() {
    if (count == 0) throw std::out_of_range("MinHeap::pop: heap rong");
    data[0] = data[count - 1];
    count--;
    if (count > 0) siftDown(0);
}

template <typename T, typename Compare>
T& MinHeap<T, Compare>::top() {
    if (count == 0) throw std::out_of_range("MinHeap::top: heap rong");
    return data[0];
}

template <typename T, typename Compare>
const T& MinHeap<T, Compare>::top() const {
    if (count == 0) throw std::out_of_range("MinHeap::top: heap rong");
    return data[0];
}

template <typename T, typename Compare>
int MinHeap<T, Compare>::size() const { return count; }

template <typename T, typename Compare>
bool MinHeap<T, Compare>::empty() const { return count == 0; }

template <typename T, typename Compare>
void MinHeap<T, Compare>::clear() { count = 0; }

#endif // DS_MINHEAP_CPP
