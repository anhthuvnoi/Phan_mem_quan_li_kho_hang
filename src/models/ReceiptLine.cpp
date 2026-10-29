#include "models/ReceiptLine.h"

ReceiptLine::ReceiptLine() : productId(), batchId(), qty(0), unitPrice(0), mfgDate(), expDate() {}

ReceiptLine::ReceiptLine(const std::string& productId, int qty, double unitPrice)
    : productId(productId), batchId(), qty(qty), unitPrice(unitPrice), mfgDate(), expDate() {}

std::string ReceiptLine::getProductId() const { return productId; }
std::string ReceiptLine::getBatchId() const { return batchId; }
int ReceiptLine::getQty() const { return qty; }
double ReceiptLine::getUnitPrice() const { return unitPrice; }
Date ReceiptLine::getMfgDate() const { return mfgDate; }
Date ReceiptLine::getExpDate() const { return expDate; }

void ReceiptLine::setBatchInfo(const std::string& b, const Date& mfg, const Date& exp) {
    batchId = b;
    mfgDate = mfg;
    expDate = exp;
}

double ReceiptLine::lineTotal() const { return qty * unitPrice; }

bool ReceiptLine::operator==(const ReceiptLine& o) const {
    return productId == o.productId && batchId == o.batchId && qty == o.qty &&
           unitPrice == o.unitPrice && mfgDate == o.mfgDate && expDate == o.expDate;
}
