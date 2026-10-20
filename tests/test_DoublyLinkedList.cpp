#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "ds/DoublyLinkedList.h"

template <typename L>
static bool same(const L& l, const int* arr, int n) {
    if (l.size() != n) return false;
    int i = 0;
    for (int x : l) if (x != arr[i++]) return false;
    return true;
}

int main() {
    // 1. Rỗng
    DoublyLinkedList<int> l;
    assert(l.empty() && l.size() == 0 && l.begin() == l.end());
    bool thrown = false;
    try { l.front(); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
    thrown = false;
    try { l.popBack(); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);

    // 2. push / pop hai đầu
    l.pushBack(2); l.pushBack(3); l.pushFront(1);          // 1 2 3
    { int e[] = {1, 2, 3}; assert(same(l, e, 3)); }
    assert(l.front() == 1 && l.back() == 3);
    l.popFront(); l.popBack();                              // 2
    assert(l.size() == 1 && l.front() == 2 && l.back() == 2);
    l.popBack();
    assert(l.empty());
    l.pushFront(9);                                         // head/tail phải hợp lệ sau khi rỗng
    assert(l.front() == 9 && l.back() == 9);
    l.clear();

    // 3. at / insertAt / removeAt / indexOf
    for (int i = 0; i < 10; i++) l.pushBack(i * 10);        // 0 10 ... 90
    assert(l.at(0) == 0 && l.at(4) == 40 && l.at(9) == 90 && l.at(7) == 70);
    thrown = false;
    try { l.at(10); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
    assert(l.insertAt(0, -1) && l.insertAt(11, 100) && l.insertAt(5, 35));
    assert(!l.insertAt(-1, 0) && !l.insertAt(100, 0));
    { int e[] = {-1, 0, 10, 20, 30, 35, 40, 50, 60, 70, 80, 90, 100}; assert(same(l, e, 13)); }
    assert(l.indexOf(35) == 5 && l.indexOf(999) == -1 && l.contains(100));
    assert(l.removeAt(0) && l.removeAt(l.size() - 1) && l.removeAt(4));
    assert(!l.removeAt(50));
    { int e[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90}; assert(same(l, e, 10)); }

    // 4. remove theo giá trị (đầu, giữa, cuối, không có)
    assert(l.remove(0) && l.remove(50) && l.remove(90) && !l.remove(12345));
    { int e[] = {10, 20, 30, 40, 60, 70, 80}; assert(same(l, e, 7)); }

    // 5. Iterator hai chiều, sửa qua iterator, erase khi duyệt
    for (auto it = l.begin(); it != l.end(); ++it) *it += 1;     // 11 21 31 41 61 71 81
    for (auto it = l.begin(); it != l.end();) {
        if (*it % 20 == 1) it = l.erase(it);                     // bỏ 21, 41, 61, 81
        else ++it;
    }
    { int e[] = {11, 31, 71}; assert(same(l, e, 3)); }
    assert(l.front() == 11 && l.back() == 71);

    // 6. forEach
    int sum = 0;
    l.forEach([&](int& x) { sum += x; x = 0; });
    assert(sum == 113 && l.at(1) == 0);
    const DoublyLinkedList<int>& cl = l;
    int cnt = 0;
    cl.forEach([&](const int&) { cnt++; });
    assert(cnt == 3);

    // 7. Copy độc lập
    DoublyLinkedList<std::string> a;
    a.pushBack("L1"); a.pushBack("L2");
    DoublyLinkedList<std::string> b(a);
    b.pushBack("L3"); b.at(0) = "X";
    assert(a.size() == 2 && a.at(0) == "L1" && b.size() == 3);
    DoublyLinkedList<std::string> c;
    c.pushBack("old");
    c = a; c = c;
    assert(c.size() == 2 && c.back() == "L2");

    // 8. Con trỏ (như DoublyLinkedList<Batch*>) — danh sách không delete
    DoublyLinkedList<int*> ptrs;
    int x = 1, y = 2;
    ptrs.pushBack(&x); ptrs.pushBack(&y);
    assert(ptrs.remove(&x) && *ptrs.front() == 2);

    // 9. Nhiều phần tử
    DoublyLinkedList<int> big;
    for (int i = 0; i < 100000; i++) big.pushBack(i);
    assert(big.size() == 100000 && big.at(99999) == 99999 && big.at(50000) == 50000);
    while (!big.empty()) big.popFront();

    std::cout << "test_DoublyLinkedList: OK\n";
    return 0;
}
