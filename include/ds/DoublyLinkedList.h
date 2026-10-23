#ifndef DS_DOUBLYLINKEDLIST_H
#define DS_DOUBLYLINKEDLIST_H

// Danh sách liên kết đôi — dùng cho: dòng phiếu nhập/xuất, danh sách lô của mặt hàng
// TODO: cài đặt toàn bộ trong file .h (template)
template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& d) : data(d), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int count;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void pushFront(const T& value);
    void pushBack(const T& value);
    void popFront();
    void popBack();
    bool remove(const T& value);
    T& front();
    T& back();
    T& at(int index) const;
    int size() const;
    bool empty() const;
    void clear();
    void forEach(void (*visit)(const T& value, void* ctx), void* ctx) const;   // duyệt từ đầu đến cuối
};
#include "../../src/ds/DoublyLinkedList.cpp"
#endif // DS_DOUBLYLINKEDLIST_H
