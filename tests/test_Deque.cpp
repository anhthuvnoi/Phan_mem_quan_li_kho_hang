#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "ds/Deque.h"

namespace {
struct PickOrder {          // tên riêng: tránh trùng với lớp Order thật (models/Order.h)
    std::string id;
    bool vip;
};
} // namespace

int main() {
    // 1. Rỗng
    Deque<int> d(4);
    assert(d.empty());
    bool thrown = false;
    try { d.front(); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
    thrown = false;
    try { d.popFront(); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);

    // 2. Hai đầu + vòng qua cuối mảng
    d.pushBack(2); d.pushBack(3); d.pushFront(1); d.pushFront(0);   // 0 1 2 3 (đầy cap 4)
    assert(d.size() == 4 && d.front() == 0 && d.back() == 3);
    for (int i = 0; i < 4; i++) assert(d.at(i) == i);
    d.pushBack(4);                                                    // resize khi headIdx != 0
    for (int i = 0; i < 5; i++) assert(d.at(i) == i);
    d.popFront(); d.popBack();                                        // 1 2 3
    assert(d.front() == 1 && d.back() == 3 && d.size() == 3);
    thrown = false;
    try { d.at(3); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);

    // 3. Xoay vòng nhiều lần (đẩy sau, lấy trước) với cap nhỏ
    Deque<int> q(3);
    int next = 0, expect = 0;
    for (int round = 0; round < 1000; round++) {
        q.pushBack(next++);
        q.pushBack(next++);
        assert(q.front() == expect++); q.popFront();
    }
    assert(q.size() == 1000);
    while (!q.empty()) { assert(q.front() == expect++); q.popFront(); }

    // 4. Như hàng đợi soạn đơn: VIP lên đầu
    Deque<PickOrder> orders;
    PickOrder list[] = {{"DH1", false}, {"DH2", true}, {"DH3", false}, {"DH4", true}};
    for (const PickOrder& o : list) {
        if (o.vip) orders.pushFront(o);
        else orders.pushBack(o);
    }
    const char* exp[] = {"DH4", "DH2", "DH1", "DH3"};
    for (int i = 0; i < 4; i++) { assert(orders.front().id == exp[i]); orders.popFront(); }

    // 5. Copy độc lập, clear
    Deque<int> a;
    a.pushFront(2); a.pushFront(1); a.pushBack(3);     // 1 2 3, headIdx != 0
    Deque<int> b(a);
    b.popFront(); b.at(0) = 99;
    assert(a.size() == 3 && a.at(0) == 1 && a.at(1) == 2 && b.at(0) == 99);
    Deque<int> c;
    c = a; c = c;
    assert(c.size() == 3 && c.back() == 3);
    c.clear();
    assert(c.empty());
    c.pushFront(5);
    assert(c.front() == 5 && c.back() == 5);

    std::cout << "test_Deque: OK\n";
    return 0;
}
