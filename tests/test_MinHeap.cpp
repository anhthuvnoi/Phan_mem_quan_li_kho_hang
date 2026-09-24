#include <cassert>
#include <iostream>
#include "ds/MinHeap.h"

// Mở khoá test này sau khi cài đặt xong MinHeap.h
int main() {
    MinHeap<int> h;
    h.push(5); h.push(2); h.push(8); h.push(1);
    assert(h.top() == 1); h.pop();
    assert(h.top() == 2); h.pop();
    assert(h.size() == 2);
    std::cout << "test_MinHeap: OK\n";
    return 0;
}
