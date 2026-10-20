#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "ds/FenwickTree.h"
#include "models/Date.h"

static bool eq(double a, double b) { return std::fabs(a - b) < 1e-6; }

int main() {
    // 1. Cơ bản
    FenwickTree f(10);
    assert(f.size() == 10);
    assert(eq(f.prefixSum(10), 0));
    assert(f.add(1, 5));
    assert(f.add(3, 2.5));
    assert(f.add(10, 1));
    assert(f.add(3, 1.5));                      // cộng dồn cùng ngày
    assert(eq(f.prefixSum(1), 5));
    assert(eq(f.prefixSum(3), 9));
    assert(eq(f.rangeSum(2, 3), 4));
    assert(eq(f.rangeSum(4, 9), 0));
    assert(eq(f.rangeSum(1, 10), 10));
    assert(eq(f.pointQuery(3), 4));
    assert(f.add(1, -2));                       // hoàn tiền / điều chỉnh
    assert(eq(f.pointQuery(1), 3));

    // 2. Chỉ số ngoài phạm vi
    assert(!f.add(0, 100) && !f.add(11, 100) && !f.add(-1, 100));
    assert(eq(f.prefixSum(0), 0) && eq(f.prefixSum(-5), 0));
    assert(eq(f.prefixSum(1000), f.prefixSum(10)));
    assert(eq(f.rangeSum(5, 2), 0));
    assert(eq(f.rangeSum(-3, 100), f.prefixSum(10)));

    // 3. So với cách cộng trâu
    const int N = 1000;
    FenwickTree g(N);
    double brute[N + 1] = {0};
    std::srand(123);
    for (int k = 0; k < 5000; k++) {
        int idx = std::rand() % N + 1;
        double v = (std::rand() % 10000) / 100.0;
        g.add(idx, v);
        brute[idx] += v;
    }
    for (int k = 0; k < 2000; k++) {
        int l = std::rand() % N + 1, r = std::rand() % N + 1;
        if (l > r) { int t = l; l = r; r = t; }
        double s = 0;
        for (int i = l; i <= r; i++) s += brute[i];
        assert(eq(g.rangeSum(l, r), s));
    }

    // 4. Copy độc lập, reset
    FenwickTree h(g);
    h.add(1, 1000);
    assert(eq(h.pointQuery(1), g.pointQuery(1) + 1000));
    FenwickTree z(1);
    z = g;
    assert(z.size() == N && eq(z.prefixSum(N), g.prefixSum(N)));
    z.reset();
    assert(eq(z.prefixSum(N), 0));

    // 5. Dùng với Date (giống ReportService)
    FenwickTree rev(20000);
    rev.add(Date(1, 10, 2026).toDayIndex(), 1500000);
    rev.add(Date(15, 10, 2026).toDayIndex(), 2500000);
    rev.add(Date(1, 11, 2026).toDayIndex(), 999);
    assert(eq(rev.rangeSum(Date(1, 10, 2026).toDayIndex(), Date(31, 10, 2026).toDayIndex()), 4000000));

    std::cout << "test_FenwickTree: OK\n";
    return 0;
}
