#ifndef DS_FENWICKTREE_H
#define DS_FENWICKTREE_H

class FenwickTree {
private:
    double* tree;    
    int n;

    static int lowbit(int i) { return i & (-i); }

public:
    FenwickTree(int size);
    FenwickTree(const FenwickTree& other);
    FenwickTree& operator=(const FenwickTree& other);
    ~FenwickTree();

    bool add(int index, double delta);        // cộng doanh thu vào ngày index
    double prefixSum(int index) const;        // tổng [1..index]
    double rangeSum(int left, int right) const;
    double pointQuery(int index) const;       // giá trị tại đúng 1 ngày
    int size() const;
    void reset();                             // đưa mọi giá trị về 0
};

#endif // DS_FENWICKTREE_H
