#include "ds/FenwickTree.h"

// tree[i] lưu tổng đoạn (i - lowbit(i), i], lowbit(i) = i & (-i). Chỉ số 1..n, tree[0] bỏ trống.

FenwickTree::FenwickTree(int size) : tree(nullptr), n(size > 0 ? size : 0) {
    tree = new double[n + 1];
    for (int i = 0; i <= n; i++) tree[i] = 0.0;
}

FenwickTree::~FenwickTree() {
    delete[] tree;
}

// Chỉ số ngoài [1, n] bị bỏ qua
void FenwickTree::add(int index, double delta) {
    if (index < 1 || index > n) return;
    for (int i = index; i <= n; i += i & (-i)) tree[i] += delta;
}

// Tổng [1..index]; index <= 0 -> 0, index > n -> tính như n
double FenwickTree::prefixSum(int index) const {
    if (index > n) index = n;
    double sum = 0.0;
    for (int i = index; i > 0; i -= i & (-i)) sum += tree[i];
    return sum;
}

double FenwickTree::rangeSum(int left, int right) const {
    if (left < 1) left = 1;
    if (right > n) right = n;
    if (left > right) return 0.0;
    return prefixSum(right) - prefixSum(left - 1);
}
