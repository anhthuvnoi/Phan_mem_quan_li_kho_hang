#include "ds/FenwickTree.h"

// khoi tao cay
FenwickTree::FenwickTree(int size) : tree(nullptr), n(size > 0 ? size : 0) {
    tree = new double[n + 1];
    for (int i = 0; i <= n; i++) tree[i] = 0.0;
}

// ham sao chep
FenwickTree::FenwickTree(const FenwickTree& other) : tree(new double[other.n + 1]), n(other.n) {
    for (int i = 0; i <= n; i++) tree[i] = other.tree[i];
}

// toan tu gan =
FenwickTree& FenwickTree::operator=(const FenwickTree& other) {
    if (this != &other) {
        double* newTree = new double[other.n + 1];
        for (int i = 0; i <= other.n; i++) newTree[i] = other.tree[i];
        delete[] tree;
        tree = newTree;
        n = other.n;
    }
    return *this;
}

// ham huy

FenwickTree::~FenwickTree() {
    delete[] tree;
}

// cap nhat phan tu moi add
// Hàm lowbit(i) lấy giá trị của bit 1 thấp nhất trong biểu diễn nhị phân của i.
bool FenwickTree::add(int index, double delta) {
    if (index < 1 || index > n) return false;
    for (int i = index; i <= n; i += lowbit(i)) tree[i] += delta;
    return true;
}

// tinh tong tu dau den vi tri index
double FenwickTree::prefixSum(int index) const {
    if (index > n) index = n;
    double sum = 0.0;
    for (int i = index; i > 0; i -= lowbit(i)) sum += tree[i];
    return sum;
}

// tinh tong doan range sum
double FenwickTree::rangeSum(int left, int right) const {
    if (left < 1) left = 1;
    if (right > n) right = n;
    if (left > right) return 0.0;
    return prefixSum(right) - prefixSum(left - 1);
}

// lay gia tri tai mot vi tri
double FenwickTree::pointQuery(int index) const {
    return rangeSum(index, index);
}

//lay kich thuoc
int FenwickTree::size() const {
    return n;
}
// xoa du lieu
void FenwickTree::reset() {
    for (int i = 0; i <= n; i++) tree[i] = 0.0;
}

