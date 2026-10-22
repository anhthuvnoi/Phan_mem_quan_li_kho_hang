#include "ds/MaxHeap.h"
#include <stdexcept>
#include <string>
#include <utility>

// Giống MinHeap nhưng đỉnh là phần tử LỚN nhất (so sánh bằng operator< của T)

template <typename T>
MaxHeap<T>::MaxHeap(int cap) : data(nullptr), capacity(cap > 0 ? cap : 1), count(0) {
    data = new T[capacity];
}

template <typename T>
MaxHeap<T>::~MaxHeap() {
    delete[] data;
}

template <typename T>
void MaxHeap<T>::resize() {
    int newCap = capacity * 2;
    T* newData = new T[newCap];
    for (int i = 0; i < count; i++) newData[i] = data[i];
    delete[] data;
    data = newData;
    capacity = newCap;
}

template <typename T>
void MaxHeap<T>::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!(data[parent] < data[i])) break;      // cha đã >= con
        T tmp = data[i];
        data[i] = data[parent];
        data[parent] = tmp;
        i = parent;
    }
}

template <typename T>
void MaxHeap<T>::siftDown(int i) {
    while (true) {
        int left = 2 * i + 1, right = 2 * i + 2, largest = i;
        if (left < count && data[largest] < data[left]) largest = left;
        if (right < count && data[largest] < data[right]) largest = right;
        if (largest == i) break;
        T tmp = data[i];
        data[i] = data[largest];
        data[largest] = tmp;
        i = largest;
    }
}

template <typename T>
void MaxHeap<T>::push(const T& value) {
    if (count == capacity) resize();
    data[count] = value;
    siftUp(count);
    count++;
}

template <typename T>
void MaxHeap<T>::pop() {
    if (count == 0) throw std::out_of_range("MaxHeap::pop: heap rong");
    data[0] = data[count - 1];
    count--;
    if (count > 0) siftDown(0);
}

template <typename T>
T& MaxHeap<T>::top() {
    if (count == 0) throw std::out_of_range("MaxHeap::top: heap rong");
    return data[0];
}

template <typename T>
int MaxHeap<T>::size() const { return count; }

template <typename T>
bool MaxHeap<T>::empty() const { return count == 0; }

// ---------- Các kiểu được dùng trong dự án ----------
template class MaxHeap<int>;
template class MaxHeap<double>;
// InventoryService: (số lượng tồn, mã kho) -> đỉnh là kho tồn nhiều nhất
template class MaxHeap<std::pair<int, std::string>>;
