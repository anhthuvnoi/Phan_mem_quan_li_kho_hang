#ifndef DS_COMPARE_H
#define DS_COMPARE_H

// Bộ so sánh mặc định dùng chung cho MinHeap / MaxHeap: dùng operator< của T
template <typename T>
struct DefaultLess {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

#endif // DS_COMPARE_H
