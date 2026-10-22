#ifndef DS_MAXHEAP_H
#define DS_MAXHEAP_H

// Max-Heap — dùng cho: gợi ý kho nguồn điều chuyển (kho có tồn cao nhất)
template <typename T>
class MaxHeap {
private:
    T* data;
    int capacity;
    int count;

    void siftUp(int i);
    void siftDown(int i);
    void resize();

public:
    MaxHeap(int cap = 16);
    ~MaxHeap();

    void push(const T& value);
    void pop();
    T& top();
    int size() const;
    bool empty() const;
};

#endif // DS_MAXHEAP_H
