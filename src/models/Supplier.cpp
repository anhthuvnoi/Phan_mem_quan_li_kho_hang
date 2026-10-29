#include "models/Supplier.h"
#include <sstream>

// File: id|name|phone|address|email|taxCode

namespace {
// Tách line theo '|' vào f[0..n-1] (bỏ '\r' của file Windows)
void splitFields(const std::string& line, std::string f[], int n) {
    std::istringstream ss(line);
    for (int i = 0; i < n; i++) {
        if (!std::getline(ss, f[i], '|')) f[i].clear();
        if (!f[i].empty() && f[i].back() == '\r') f[i].pop_back();
    }
}
} // namespace

Supplier::Supplier() : Partner(), taxCode() {}

Supplier::Supplier(const std::string& id, const std::string& name, const std::string& phone,
                   const std::string& address, const std::string& email, const std::string& taxCode)
    : Partner(id, name, phone, address, email), taxCode(taxCode) {}

std::string Supplier::getTaxCode() const { return taxCode; }
void Supplier::setTaxCode(const std::string& t) { taxCode = t; }

std::string Supplier::toString() const {
    return "[NCC] " + id + " - " + name + " | SDT: " + phone + " | DC: " + address +
           " | Email: " + email + " | MST: " + taxCode;
}

std::string Supplier::toFileString() const {
    return id + "|" + name + "|" + phone + "|" + address + "|" + email + "|" + taxCode;
}

void Supplier::fromFileString(const std::string& line) {
    std::string f[6];
    splitFields(line, f, 6);
    id = f[0];
    name = f[1];
    phone = f[2];
    address = f[3];
    email = f[4];
    taxCode = f[5];
}
