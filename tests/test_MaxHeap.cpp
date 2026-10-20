#include <cassert>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include "ds/MaxHeap.h"

// Mô phỏng gợi ý kho nguồn: kho tồn nhiều nhất lên đầu
namespace {
struct WhStock {
    int qty;
    std::string warehouseId;
};
struct WhStockLess {
    bool operator()(const WhStock& a, const WhStock& b) const { return a.qty < b.qty; }
};
} // namespace

int main() {
    // 1. Cơ bản
    MaxHeap<int> h;
    assert(h.empty());
    h.push(5); h.push(2); h.push(8); h.push(1);
    assert(h.top() == 8); h.pop();
    assert(h.top() == 5); h.pop();
    assert(h.size() == 2);
    assert(h.top() == 2); h.pop();
    assert(h.top() == 1); h.pop();
    assert(h.empty());

    // 2. Rỗng -> ném ngoại lệ
    bool thrown = false;
    try { h.top(); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
    thrown = false;
    try { h.pop(); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);

    // 3. Nhiều phần tử -> lấy ra phải không tăng
    MaxHeap<int> big(1);
    const int N = 5000;
    std::srand(7);
    for (int i = 0; i < N; i++) big.push(std::rand() % 1000 - 500);
    int prev = 1000000;
    for (int i = 0; i < N; i++) {
        assert(big.top() <= prev);
        prev = big.top();
        big.pop();
    }
    assert(big.empty());

    // 4. Comparator tuỳ chỉnh
    MaxHeap<WhStock, WhStockLess> wh;
    wh.push({120, "K01"}); wh.push({500, "K02"}); wh.push({0, "K03"}); wh.push({300, "K04"});
    assert(wh.top().warehouseId == "K02"); wh.pop();
    assert(wh.top().warehouseId == "K04"); wh.pop();
    assert(wh.top().warehouseId == "K01"); wh.pop();
    assert(wh.top().warehouseId == "K03"); wh.pop();
    assert(wh.empty());

    // 5. Copy độc lập
    MaxHeap<int> a;
    a.push(1); a.push(9);
    MaxHeap<int> b = a;
    b.pop();
    assert(a.top() == 9 && b.top() == 1);
    MaxHeap<int> c;
    c = a;
    c.push(100);
    assert(c.top() == 100 && a.top() == 9 && a.size() == 2);

    std::cout << "test_MaxHeap: OK\n";
    return 0;
}
