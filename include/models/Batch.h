#ifndef MODELS_BATCH_H
#define MODELS_BATCH_H

#include <string>
#include "models/Date.h"

// Lô hàng: mỗi lần nhập tạo 1 lô, có HSD và giá vốn riêng
class Batch {
private:
    std::string batchId;
    std::string productId;
    std::string warehouseId;
    Date mfgDate;         // ngày sản xuất
    Date expDate;         // hạn sử dụng
    double costPrice;     // giá vốn
    int qtyIn;            // số lượng nhập
    int qtyRemain;        // số lượng còn lại

public:
    Batch();
    Batch(const std::string& batchId, const std::string& productId, const std::string& warehouseId,
          const Date& mfg, const Date& exp, double costPrice, int qty);

    std::string getBatchId() const;
    std::string getProductId() const;
    std::string getWarehouseId() const;
    Date getMfgDate() const;
    Date getExpDate() const;
    double getCostPrice() const;
    int getQtyIn() const;
    int getQtyRemain() const;

    int take(int qty);                        // lấy tối đa qty, trả về số thực lấy được
    void addQty(int qty);
    bool isExpired(const Date& today) const;
    int daysToExpire(const Date& today) const;

    bool operator<(const Batch& other) const; // so sánh theo HSD -> dùng cho Min-Heap (FEFO)

    std::string toString() const;
    std::string toFileString() const;
    void fromFileString(const std::string& line);
};

// So sánh con trỏ Batch* theo HSD (heap lưu con trỏ)
struct BatchPtrLess {
    bool operator()(const Batch* a, const Batch* b) const { return *a < *b; }
};

#endif // MODELS_BATCH_H
