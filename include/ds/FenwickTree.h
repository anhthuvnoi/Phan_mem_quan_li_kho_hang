#ifndef DS_FENWICKTREE_H
#define DS_FENWICKTREE_H

// Cây Fenwick (BIT) — dùng cho: tổng doanh thu theo khoảng ngày
// Chỉ số = số ngày tính từ mốc (Date::toDayIndex)
class FenwickTree {
private:
    double* tree;
    int n;

public:
    FenwickTree(int size);
    ~FenwickTree();

    void add(int index, double delta);        // cộng doanh thu vào ngày index
    double prefixSum(int index) const;        // tổng [1..index]
    double rangeSum(int left, int right) const;
};

#endif // DS_FENWICKTREE_H
