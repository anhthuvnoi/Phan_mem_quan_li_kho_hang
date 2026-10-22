#include "ds/MinHeap.h"
#include <stdexcept>
#include <string>

// Mảng, gốc ở chỉ số 0: cha(i) = (i-1)/2, con trái = 2i+1, con phải = 2i+2
// cmp(a, b) == true nghĩa là a "nhỏ hơn" b -> đỉnh luôn là phần tử nhỏ nhất

template <typename T, typename Compare>
MinHeap<T, Compare>::MinHeap(int cap) : data(nullptr), capacity(cap > 0 ? cap : 1), count(0), cmp() {
    data = new T[capacity];
}

template <typename T, typename Compare>
MinHeap<T, Compare>::~MinHeap() {
    delete[] data;
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

// Vun lên: con nhỏ hơn cha thì đổi chỗ, lặp đến gốc
template <typename T, typename Compare>
void MinHeap<T, Compare>::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!cmp(data[i], data[parent])) break;
        T tmp = data[i];
        data[i] = data[parent];
        data[parent] = tmp;
        i = parent;
    }
}

// Vun xuống: đổi với con nhỏ hơn cho đến khi nhỏ hơn cả hai con
template <typename T, typename Compare>
void MinHeap<T, Compare>::siftDown(int i) {
    while (true) {
        int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
        if (left < count && cmp(data[left], data[smallest])) smallest = left;
        if (right < count && cmp(data[right], data[smallest])) smallest = right;
        if (smallest == i) break;
        T tmp = data[i];
        data[i] = data[smallest];
        data[smallest] = tmp;
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
    data[0] = data[count - 1];       // đưa phần tử cuối lên gốc rồi vun xuống
    count--;
    if (count > 0) siftDown(0);
}

template <typename T, typename Compare>
T& MinHeap<T, Compare>::top() {
    if (count == 0) throw std::out_of_range("MinHeap::top: heap rong");
    return data[0];
}

template <typename T, typename Compare>
int MinHeap<T, Compare>::size() const { return count; }

template <typename T, typename Compare>
bool MinHeap<T, Compare>::empty() const { return count == 0; }

// ---------- Các kiểu được dùng trong dự án ----------
#include "models/Batch.h"

template class MinHeap<int>;
template class MinHeap<double>;
template class MinHeap<Batch*, BatchPtrLess>;     // StockItem (FEFO), AlertService (cận date)
