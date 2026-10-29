#include "models/Product.h"
#include <iomanip>
#include <sstream>

// File: id|name|unit|category|salePrice|minStock

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
// 32000 -> "32000", 12.5 -> "12.5" (không ra dạng 3.2e+04)
std::string num(double x) {
    std::ostringstream os;
    os << std::setprecision(15) << x;
    return os.str();
}
} // namespace

Product::Product() : Entity(), unit(), category(), salePrice(0), minStock(0) {}

Product::Product(const std::string& id, const std::string& name, const std::string& unit,
                 const std::string& category, double salePrice, int minStock)
    : Entity(id, name), unit(unit), category(category),
      salePrice(salePrice < 0 ? 0 : salePrice), minStock(minStock < 0 ? 0 : minStock) {}

std::string Product::getUnit() const { return unit; }
std::string Product::getCategory() const { return category; }
double Product::getSalePrice() const { return salePrice; }
int Product::getMinStock() const { return minStock; }

bool Product::setSalePrice(double p) {
    if (p < 0) return false;
    salePrice = p;
    return true;
}

bool Product::setMinStock(int m) {
    if (m < 0) return false;
    minStock = m;
    return true;
}

std::string Product::toString() const {
    return "[MH] " + id + " - " + name + " | DVT: " + unit + " | Loai: " + category +
           " | Gia ban: " + num(salePrice) + " | Ton toi thieu: " + std::to_string(minStock);
}

std::string Product::toFileString() const {
    return id + "|" + name + "|" + unit + "|" + category + "|" + num(salePrice) + "|" + std::to_string(minStock);
}

void Product::fromFileString(const std::string& line) {
    std::string f[6];
    splitFields(line, f, 6);
    id = f[0];
    name = f[1];
    unit = f[2];
    category = f[3];
    salePrice = toDouble(f[4]);
    if (salePrice < 0) salePrice = 0;
    minStock = toInt(f[5]);
    if (minStock < 0) minStock = 0;
}
