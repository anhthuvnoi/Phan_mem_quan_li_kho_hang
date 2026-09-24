#ifndef DS_DEQUE_H
#define DS_DEQUE_H

// Hàng đợi hai đầu — dùng cho: hàng đợi đơn chờ soạn (VIP/khẩn vào đầu, thường vào cuối)
template <typename T>
class Deque {
private:
    T* data;       // mảng vòng
    int capacity;
    int headIdx;
    int count;
    void resize();

public:
    Deque(int cap = 16);
    ~Deque();

    void pushFront(const T& value);
    void pushBack(const T& value);
    void popFront();
    void popBack();
    T& front();
    T& back();
    int size() const;
    bool empty() const;
};

#endif // DS_DEQUE_H
