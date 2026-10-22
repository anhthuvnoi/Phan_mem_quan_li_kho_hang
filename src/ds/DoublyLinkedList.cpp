#include "ds/DoublyLinkedList.h"
#include <stdexcept>
#include <string>

// Cài đặt DoublyLinkedList<T>. Vì thân hàm template nằm ở .cpp, cuối file phải
// "khai báo tường minh" (explicit instantiation) các kiểu T mà dự án dùng.

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    clear();
}

template <typename T>
void DoublyLinkedList<T>::pushFront(const T& value) {
    Node* n = new Node(value);
    n->next = head;
    if (head != nullptr) head->prev = n;
    else tail = n;                  // danh sách đang rỗng
    head = n;
    count++;
}

template <typename T>
void DoublyLinkedList<T>::pushBack(const T& value) {
    Node* n = new Node(value);
    n->prev = tail;
    if (tail != nullptr) tail->next = n;
    else head = n;
    tail = n;
    count++;
}

template <typename T>
void DoublyLinkedList<T>::popFront() {
    if (head == nullptr) throw std::out_of_range("DoublyLinkedList::popFront: danh sach rong");
    Node* old = head;
    head = head->next;
    if (head != nullptr) head->prev = nullptr;
    else tail = nullptr;
    delete old;
    count--;
}

template <typename T>
void DoublyLinkedList<T>::popBack() {
    if (tail == nullptr) throw std::out_of_range("DoublyLinkedList::popBack: danh sach rong");
    Node* old = tail;
    tail = tail->prev;
    if (tail != nullptr) tail->next = nullptr;
    else head = nullptr;
    delete old;
    count--;
}

// Xoá phần tử ĐẦU TIÊN có giá trị bằng value
template <typename T>
bool DoublyLinkedList<T>::remove(const T& value) {
    for (Node* p = head; p != nullptr; p = p->next) {
        if (p->data == value) {
            if (p->prev != nullptr) p->prev->next = p->next;
            else head = p->next;
            if (p->next != nullptr) p->next->prev = p->prev;
            else tail = p->prev;
            delete p;
            count--;
            return true;
        }
    }
    return false;
}

template <typename T>
T& DoublyLinkedList<T>::front() {
    if (head == nullptr) throw std::out_of_range("DoublyLinkedList::front: danh sach rong");
    return head->data;
}

template <typename T>
T& DoublyLinkedList<T>::back() {
    if (tail == nullptr) throw std::out_of_range("DoublyLinkedList::back: danh sach rong");
    return tail->data;
}

// Đi từ đầu gần hơn: nửa đầu đi từ head, nửa sau đi lùi từ tail
template <typename T>
T& DoublyLinkedList<T>::at(int index) {
    if (index < 0 || index >= count) throw std::out_of_range("DoublyLinkedList::at: chi so khong hop le");
    Node* p;
    if (index < count / 2) {
        p = head;
        for (int i = 0; i < index; i++) p = p->next;
    } else {
        p = tail;
        for (int i = count - 1; i > index; i--) p = p->prev;
    }
    return p->data;
}

template <typename T>
int DoublyLinkedList<T>::size() const { return count; }

template <typename T>
bool DoublyLinkedList<T>::empty() const { return count == 0; }

template <typename T>
void DoublyLinkedList<T>::clear() {
    Node* p = head;
    while (p != nullptr) {
        Node* next = p->next;
        delete p;
        p = next;
    }
    head = tail = nullptr;
    count = 0;
}

template <typename T>
void DoublyLinkedList<T>::forEach(void (*visit)(const T& value, void* ctx), void* ctx) const {
    if (visit == nullptr) return;
    for (Node* p = head; p != nullptr; p = p->next) visit(p->data, ctx);
}

// ---------- Các kiểu được dùng trong dự án ----------
#include "models/Batch.h"
#include "models/ReceiptLine.h"
#include "models/Receipt.h"
#include "models/Order.h"

template class DoublyLinkedList<int>;
template class DoublyLinkedList<std::string>;
template class DoublyLinkedList<Batch*>;          // StockItem: danh sách lô
template class DoublyLinkedList<ReceiptLine>;     // Receipt, Order: các dòng hàng
template class DoublyLinkedList<Receipt*>;        // InventoryService: lịch sử phiếu
template class DoublyLinkedList<Order*>;
