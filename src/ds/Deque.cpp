#include <ds/Deque.h>
using namespace std;
template <typename T>
Deque<T>::Deque(int cap) : cap(cap > 0 ? cap : 16), headIdx(0), count(0) {
    data = new T[capacity];
}
template <typename T>
Deque<T>::~Deque() {
    delete[] data;
    data = nullptr;
}
template <typename T>
void Deque<T>::resize() {
    int newCapacity = capacity * 2;
    T* newData = new T[newCapacity];
    for(int i=0; i<count; i++){
        newData[i] = data[(headIdx + i) % capacity];
    }
    delete[] data;
    data = newData;
    headIdx = 0;
    capacity = newCapacity;
}
template <typename T>
void Deque<T>::pushFront(const T& value){
    if (count == capacity) {
        resize();
    }
    headIdx = (headIdx - 1 + capacity) % capacity;
    data[headIdx] = value;
    count++;
}
template <typename T>
void Deque<T>::pushBack(const T& value) {
    if (count == capacity) {
        resize();
    }
    int tailIdx = (headIdx + count) % capacity;
    data[tailIdx] = value;
    count++;
}

template <typename T>
void Deque<T>::popFront() {
    if (empty()) return;
    headIdx = (headIdx + 1) % capacity;
    count--;
}

template <typename T>
void Deque<T>::popBack() {
    if (empty()) return;
    count--;
}

template <typename T>
T& Deque<T>::front() {
    if (empty()) {
        throw out_of_range("Deque rong!");
    }
    return data[headIdx];
}
template <typename T>
T& Deque<T>::back() {
    if (empty()) {
        throw std::out_of_range("Deque rong!");
    }
    int tailIdx = (headIdx + count - 1) % capacity;
    return data[tailIdx];
}
template <typename T>
int Deque<T>::size() const {
    return count;
}

template <typename T>
bool Deque<T>::empty() const {
    return count == 0;
}