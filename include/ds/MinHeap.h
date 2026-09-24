#ifndef DS_MINHEAP_H
#define DS_MINHEAP_H

// Min-Heap — dùng cho: xuất kho FEFO (lô hết hạn sớm nhất), cảnh báo cận date
// Compare mặc định dùng operator< của T
template <typename T>
struct DefaultLess {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

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

public:
    MinHeap(int cap = 16);
    ~MinHeap();

    void push(const T& value);
    void pop();
    T& top();
    int size() const;
    bool empty() const;
};

#endif // DS_MINHEAP_H
