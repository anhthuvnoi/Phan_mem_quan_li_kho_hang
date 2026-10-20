#ifndef DS_MAXHEAP_CPP
#define DS_MAXHEAP_CPP

#include "ds/MaxHeap.h"

template <typename T, typename Compare>
MaxHeap<T, Compare>::MaxHeap(int cap)
    : data(nullptr), capacity(cap > 0 ? cap : 1), count(0), cmp() {
    data = new T[capacity];
}

template <typename T, typename Compare>
MaxHeap<T, Compare>::MaxHeap(const MaxHeap& other)
    : data(new T[other.capacity]), capacity(other.capacity), count(other.count), cmp(other.cmp) {
    for (int i = 0; i < count; i++) data[i] = other.data[i];
}

template <typename T, typename Compare>
MaxHeap<T, Compare>& MaxHeap<T, Compare>::operator=(const MaxHeap& other) {
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
MaxHeap<T, Compare>::~MaxHeap() {
    delete[] data;
}

template <typename T, typename Compare>
void MaxHeap<T, Compare>::swapAt(int i, int j) {
    T tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
}

template <typename T, typename Compare>
void MaxHeap<T, Compare>::resize() {
    int newCap = capacity * 2;
    T* newData = new T[newCap];
    for (int i = 0; i < count; i++) newData[i] = data[i];
    delete[] data;
    data = newData;
    capacity = newCap;
}

template <typename T, typename Compare>
void MaxHeap<T, Compare>::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (cmp(data[parent], data[i])) {     
            swapAt(i, parent);
            i = parent;
        } else {
            break;
        }
    }
}

template <typename T, typename Compare>
void MaxHeap<T, Compare>::siftDown(int i) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;
        if (left < count && cmp(data[largest], data[left])) largest = left;
        if (right < count && cmp(data[largest], data[right])) largest = right;
        if (largest == i) break;
        swapAt(i, largest);
        i = largest;
    }
}

template <typename T, typename Compare>
void MaxHeap<T, Compare>::push(const T& value) {
    if (count == capacity) resize();
    data[count] = value;
    siftUp(count);
    count++;
}

template <typename T, typename Compare>
void MaxHeap<T, Compare>::pop() {
    if (count == 0) throw std::out_of_range("MaxHeap::pop: heap rong");
    data[0] = data[count - 1];
    count--;
    if (count > 0) siftDown(0);
}

template <typename T, typename Compare>
T& MaxHeap<T, Compare>::top() {
    if (count == 0) throw std::out_of_range("MaxHeap::top: heap rong");
    return data[0];
}

template <typename T, typename Compare>
const T& MaxHeap<T, Compare>::top() const {
    if (count == 0) throw std::out_of_range("MaxHeap::top: heap rong");
    return data[0];
}

template <typename T, typename Compare>
int MaxHeap<T, Compare>::size() const { return count; }

template <typename T, typename Compare>
bool MaxHeap<T, Compare>::empty() const { return count == 0; }

template <typename T, typename Compare>
void MaxHeap<T, Compare>::clear() { count = 0; }

#endif // DS_MAXHEAP_CPP
