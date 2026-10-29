#include "models/Warehouse.h"
#include <sstream>

// File: id|name|address   (tồn kho lưu riêng ở batches.txt)

namespace {
void splitFields(const std::string& line, std::string f[], int n) {
    std::istringstream ss(line);
    for (int i = 0; i < n; i++) {
        if (!std::getline(ss, f[i], '|')) f[i].clear();
        if (!f[i].empty() && f[i].back() == '\r') f[i].pop_back();
    }
}
void deleteStock(const std::string&, StockItem* const& s, void*) { delete s; }
void sumStock(const std::string&, StockItem* const& s, void* ctx) {
    *static_cast<int*>(ctx) += s->getTotalQty();
}
} // namespace

Warehouse::Warehouse() : Entity(), address(), stock() {}

Warehouse::Warehouse(const std::string& id, const std::string& name, const std::string& address)
    : Entity(id, name), address(address), stock() {}

// Warehouse sở hữu các StockItem*
Warehouse::~Warehouse() {
    stock.forEach(deleteStock, nullptr);
}

std::string Warehouse::getAddress() const { return address; }
void Warehouse::setAddress(const std::string& a) { address = a; }

StockItem* Warehouse::getStock(const std::string& productId) {
    StockItem** s = stock.find(productId);
    return s ? *s : nullptr;
}

StockItem* Warehouse::getOrCreateStock(const std::string& productId) {
    StockItem* s = getStock(productId);
    if (s == nullptr) {
        s = new StockItem(productId, id);
        stock.insert(productId, s);
    }
    return s;
}

int Warehouse::getQty(const std::string& productId) {
    StockItem* s = getStock(productId);
    return s ? s->getTotalQty() : 0;
}

std::string Warehouse::toString() const {
    int total = 0;
    stock.forEach(sumStock, &total);
    return "[Kho] " + id + " - " + name + " | DC: " + address +
           " | So mat hang: " + std::to_string(stock.size()) + " | Tong ton: " + std::to_string(total);
}

std::string Warehouse::toFileString() const {
    return id + "|" + name + "|" + address;
}

void Warehouse::fromFileString(const std::string& line) {
    std::string f[3];
    splitFields(line, f, 3);
    id = f[0];
    name = f[1];
    address = f[2];
}
