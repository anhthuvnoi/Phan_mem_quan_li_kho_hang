#include "models/Batch.h"
#include <iomanip>
#include <sstream>

// File: batchId|productId|warehouseId|mfgDate|expDate|costPrice|qtyIn|qtyRemain

namespace {
void splitFields(const std::string& line, std::string f[], int n) {
    std::istringstream ss(line);
    for (int i = 0; i < n; i++) {
        if (!std::getline(ss, f[i], '|')) f[i].clear();
        if (!f[i].empty() && f[i].back() == '\r') f[i].pop_back();
    }
}
double toDouble(const std::string& s) {
    try { return std::stod(s); } catch (...) { return 0.0; }
}
int toInt(const std::string& s) {
    try { return std::stoi(s); } catch (...) { return 0; }
}
std::string num(double x) {
    std::ostringstream os;
    os << std::setprecision(15) << x;
    return os.str();
}
} // namespace

Batch::Batch() : batchId(), productId(), warehouseId(), mfgDate(), expDate(), costPrice(0), qtyIn(0), qtyRemain(0) {}

Batch::Batch(const std::string& batchId, const std::string& productId, const std::string& warehouseId,
             const Date& mfg, const Date& exp, double costPrice, int qty)
    : batchId(batchId), productId(productId), warehouseId(warehouseId), mfgDate(mfg), expDate(exp),
      costPrice(costPrice < 0 ? 0 : costPrice), qtyIn(qty < 0 ? 0 : qty), qtyRemain(qty < 0 ? 0 : qty) {}

std::string Batch::getBatchId() const { return batchId; }
std::string Batch::getProductId() const { return productId; }
std::string Batch::getWarehouseId() const { return warehouseId; }
Date Batch::getMfgDate() const { return mfgDate; }
Date Batch::getExpDate() const { return expDate; }
double Batch::getCostPrice() const { return costPrice; }
int Batch::getQtyIn() const { return qtyIn; }
int Batch::getQtyRemain() const { return qtyRemain; }

int Batch::take(int qty) {
    if (qty <= 0) return 0;
    int taken = qty < qtyRemain ? qty : qtyRemain;
    qtyRemain -= taken;
    return taken;
}

void Batch::addQty(int qty) {
    if (qty <= 0) return;
    qtyIn += qty;
    qtyRemain += qty;
}

// Hết hạn khi HSD đã qua; đúng ngày HSD vẫn còn dùng được
bool Batch::isExpired(const Date& today) const { return expDate < today; }

// Âm nếu đã hết hạn
int Batch::daysToExpire(const Date& today) const { return expDate - today; }

// So theo HSD; cùng HSD thì theo mã lô để thứ tự ổn định
bool Batch::operator<(const Batch& other) const {
    if (!(expDate == other.expDate)) return expDate < other.expDate;
    return batchId < other.batchId;
}

std::string Batch::toString() const {
    return "[Lo] " + batchId + " | MH: " + productId + " | Kho: " + warehouseId +
           " | NSX: " + mfgDate.toString() + " | HSD: " + expDate.toString() +
           " | Gia von: " + num(costPrice) + " | Con: " + std::to_string(qtyRemain) + "/" + std::to_string(qtyIn);
}

std::string Batch::toFileString() const {
    return batchId + "|" + productId + "|" + warehouseId + "|" + mfgDate.toString() + "|" + expDate.toString() +
           "|" + num(costPrice) + "|" + std::to_string(qtyIn) + "|" + std::to_string(qtyRemain);
}

void Batch::fromFileString(const std::string& line) {
    std::string f[8];
    splitFields(line, f, 8);
    batchId = f[0];
    productId = f[1];
    warehouseId = f[2];
    mfgDate = Date::fromString(f[3]);
    expDate = Date::fromString(f[4]);
    costPrice = toDouble(f[5]);
    qtyIn = toInt(f[6]);
    qtyRemain = toInt(f[7]);
    if (qtyIn < 0) qtyIn = 0;
    if (qtyRemain < 0) qtyRemain = 0;
    if (qtyRemain > qtyIn) qtyRemain = qtyIn;
}
