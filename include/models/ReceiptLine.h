#ifndef MODELS_RECEIPTLINE_H
#define MODELS_RECEIPTLINE_H

#include <string>
#include "models/Date.h"

// Một dòng trên phiếu nhập / xuất / điều chuyển / đơn hàng
class ReceiptLine {
private:
    std::string productId;
    std::string batchId;     // phiếu nhập: lô mới; phiếu xuất: có thể để trống (FEFO tự chọn)
    int qty;
    double unitPrice;
    Date mfgDate;            // chỉ dùng cho phiếu nhập
    Date expDate;            // chỉ dùng cho phiếu nhập

public:
    ReceiptLine();
    ReceiptLine(const std::string& productId, int qty, double unitPrice);

    std::string getProductId() const;
    std::string getBatchId() const;
    int getQty() const;
    double getUnitPrice() const;
    Date getMfgDate() const;
    Date getExpDate() const;
    void setBatchInfo(const std::string& batchId, const Date& mfg, const Date& exp);

    double lineTotal() const;
    bool operator==(const ReceiptLine& other) const;
};

#endif // MODELS_RECEIPTLINE_H
