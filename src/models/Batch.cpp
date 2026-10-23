#include "models/Batch.h"
#include "models/Batch.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

Batch::Batch()
    : batchId(""), productId(""), warehouseId(""),
      mfgDate(Date()), expDate(Date()),
      costPrice(0.0), qtyIn(0), qtyRemain(0) {}

Batch::Batch(const string& batchId, const string& productId, const string& warehouseId,
             const Date& mfg, const Date& exp, double costPrice, int qty)
    : batchId(batchId), productId(productId), warehouseId(warehouseId),
      mfgDate(mfg), expDate(exp),
      costPrice(costPrice), qtyIn(qty), qtyRemain(qty) {}

string Batch::getBatchId() const {
    return batchId;
}

string Batch::getProductId() const {
    return productId;
}

string Batch::getWarehouseId() const {
    return warehouseId;
}

Date Batch::getMfgDate() const {
    return mfgDate;
}

Date Batch::getExpDate() const {
    return expDate;
}

double Batch::getCostPrice() const {
    return costPrice;
}

int Batch::getQtyIn() const {
    return qtyIn;
}

int Batch::getQtyRemain() const {
    return qtyRemain;
}

int Batch::take(int qty) {
    if (qty <= 0 || qtyRemain <= 0) {
        return 0;
    }
    int actualTaken = min(qty, qtyRemain);
    qtyRemain -= actualTaken;
    return actualTaken;
}

void Batch::addQty(int qty) {
    if (qty > 0) {
        qtyRemain += qty;
    }
}

bool Batch::isExpired(const Date& today) const {
    return expDate < today;
}

int Batch::daysToExpire(const Date& today) const {
    return expDate - today;
}

bool Batch::operator<(const Batch& other) const {
    return this->expDate < other.expDate;
}

string Batch::toString() const {
    ostringstream oss;
    oss << "Batch[ID: " << batchId
        << ", Product: " << productId
        << ", Warehouse: " << warehouseId
        << ", Exp: " << expDate.toString()
        << ", Qty: " << qtyRemain << "/" << qtyIn
        << ", Cost: " << fixed << setprecision(2) << costPrice << "]";
    return oss.str();
}

// Định dạng lưu file (batchId|productId|warehouseId|mfgDate|expDate|costPrice|qtyIn|qtyRemain)
string Batch::toFileString() const {
    ostringstream oss;
    oss << batchId << "|"
        << productId << "|"
        << warehouseId << "|"
        << mfgDate.toString() << "|"
        << expDate.toString() << "|"
        << fixed << setprecision(2) << costPrice << "|"
        << qtyIn << "|"
        << qtyRemain;
    return oss.str();
}

void Batch::fromFileString(const string& line) {
    stringstream ss(line);
    string token;

    if (getline(ss, batchId, '|')) {
        getline(ss, productId, '|');
        getline(ss, warehouseId, '|');

        string mfgStr;
        if (getline(ss, mfgStr, '|')) {
            mfgDate = Date::fromString(mfgStr);
        }

        string expStr;
        if (getline(ss, expStr, '|')) {
            expDate = Date::fromString(expStr);
        }

        if (getline(ss, token, '|')) {
            costPrice = stod(token);
        }
        if (getline(ss, token, '|')) {
            qtyIn = stoi(token);
        }
        if (getline(ss, token, '|')) {
            qtyRemain = stoi(token);
        }
    }
}
