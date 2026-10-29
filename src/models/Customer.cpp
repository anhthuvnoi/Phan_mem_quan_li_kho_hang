#include "models/Customer.h"
#include <sstream>

// File: id|name|phone|address|email|vip(0/1)

namespace {
void splitFields(const std::string& line, std::string f[], int n) {
    std::istringstream ss(line);
    for (int i = 0; i < n; i++) {
        if (!std::getline(ss, f[i], '|')) f[i].clear();
        if (!f[i].empty() && f[i].back() == '\r') f[i].pop_back();
    }
}
} // namespace

Customer::Customer() : Partner(), vip(false) {}

Customer::Customer(const std::string& id, const std::string& name, const std::string& phone,
                   const std::string& address, const std::string& email, bool vip)
    : Partner(id, name, phone, address, email), vip(vip) {}

bool Customer::isVIP() const { return vip; }
void Customer::setVIP(bool v) { vip = v; }

std::string Customer::toString() const {
    return std::string("[KH") + (vip ? " VIP" : "") + "] " + id + " - " + name +
           " | SDT: " + phone + " | DC: " + address + " | Email: " + email;
}

std::string Customer::toFileString() const {
    return id + "|" + name + "|" + phone + "|" + address + "|" + email + "|" + (vip ? "1" : "0");
}

void Customer::fromFileString(const std::string& line) {
    std::string f[6];
    splitFields(line, f, 6);
    id = f[0];
    name = f[1];
    phone = f[2];
    address = f[3];
    email = f[4];
    vip = (f[5] == "1");
}
