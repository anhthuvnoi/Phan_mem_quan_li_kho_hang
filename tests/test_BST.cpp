#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include "ds/BST.h"

int main() {
    // 1. Cơ bản: giá -> mã mặt hàng (có trùng giá)
    BST<double, std::string> t;
    assert(t.empty() && t.height() == 0);
    t.insert(15000, "SP01");
    t.insert(8000, "SP02");
    t.insert(25000, "SP03");
    t.insert(15000, "SP04");          // trùng giá
    t.insert(50000, "SP05");
    assert(t.size() == 5);
    assert(t.contains(15000) && !t.contains(16000));

    std::string r[10];
    int n = t.rangeQuery(10000, 30000, r, 10);
    assert(n == 3 && r[0] == "SP01" && r[1] == "SP04" && r[2] == "SP03");   // tăng dần theo giá
    assert(t.rangeQuery(0, 100000, r, 10) == 5 && r[0] == "SP02" && r[4] == "SP05");
    assert(t.rangeQuery(8000, 8000, r, 10) == 1 && r[0] == "SP02");         // biên đóng
    assert(t.rangeQuery(60000, 70000, r, 10) == 0);
    assert(t.rangeQuery(30000, 10000, r, 10) == 0);                         // low > high
    assert(t.rangeQuery(0, 100000, r, 2) == 2);                             // giới hạn maxResults
    assert(t.countInRange(10000, 30000) == 3);

    // 2. remove theo (key, value)
    assert(t.remove(15000, "SP01"));
    assert(!t.remove(15000, "SP01"));
    assert(!t.remove(15000, "SP99"));
    assert(!t.remove(12345, "SP04"));
    assert(t.contains(15000) && t.size() == 4);
    assert(t.remove(15000, "SP04"));
    assert(!t.contains(15000) && t.size() == 3);
    assert(t.remove(8000, "SP02"));             // xoá node có 2 con / lá
    assert(t.remove(50000, "SP05"));
    assert(t.remove(25000, "SP03"));
    assert(t.empty());

    // 3. Chèn tăng dần -> AVL vẫn cân bằng (cây thường sẽ cao 10000)
    BST<int, int> avl;
    const int N = 10000;
    for (int i = 0; i < N; i++) avl.insert(i, i);
    assert(avl.size() == N);
    assert(avl.height() <= 15);                 // ~1.44 log2(N)
    int buf[N];
    n = avl.rangeQuery(100, 199, buf, N);
    assert(n == 100);
    for (int i = 0; i < n; i++) assert(buf[i] == 100 + i);

    // 4. Xoá ngẫu nhiên + so với mảng đếm
    static int cnt[1000];
    BST<int, int> rnd;
    std::srand(2026);
    for (int i = 0; i < 5000; i++) { int k = std::rand() % 1000; rnd.insert(k, i); cnt[k]++; }
    std::srand(2026);                           // lặp lại chuỗi rand để xoá đúng cặp (k, i)
    int removedCnt = 0;
    for (int i = 0; i < 5000; i++) {
        int k = std::rand() % 1000;
        if (i % 3 == 0) { assert(rnd.remove(k, i)); cnt[k]--; removedCnt++; }
    }
    assert(rnd.size() == 5000 - removedCnt);
    assert(rnd.height() <= 20);
    for (int k = 0; k < 1000; k++) assert(rnd.countInRange(k, k) == cnt[k] && rnd.contains(k) == (cnt[k] > 0));
    int lo = 200, hi = 450, expected = 0;
    for (int k = lo; k <= hi; k++) expected += cnt[k];
    assert(rnd.countInRange(lo, hi) == expected);

    // 5. forEach theo thứ tự tăng dần
    int prev = -1, seen = 0;
    rnd.forEach([&](const int& k, const int&) { assert(k >= prev); prev = k; seen++; });
    assert(seen == rnd.size());

    // 6. Copy độc lập, clear
    BST<int, std::string> a;
    a.insert(1, "a"); a.insert(2, "b"); a.insert(2, "c");
    BST<int, std::string> b(a);
    b.remove(2, "b");
    assert(a.size() == 3 && b.size() == 2);
    BST<int, std::string> c;
    c.insert(9, "z");
    c = a; c = c;
    assert(c.size() == 3 && !c.contains(9));
    std::string out[5];
    assert(c.rangeQuery(2, 2, out, 5) == 2 && out[0] == "b" && out[1] == "c");
    c.clear();
    assert(c.empty() && !c.contains(1));

    std::cout << "test_BST: OK\n";
    return 0;
}
