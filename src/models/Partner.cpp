#include "models/Partner.h"

Partner::Partner() : Entity(), phone(), address(), email() {}

Partner::Partner(const std::string& id, const std::string& name,
                 const std::string& phone, const std::string& address, const std::string& email)
    : Entity(id, name), phone(phone), address(address), email(email) {}

Partner::~Partner() {}

std::string Partner::getPhone() const { return phone; }
std::string Partner::getAddress() const { return address; }
std::string Partner::getEmail() const { return email; }
void Partner::setPhone(const std::string& p) { phone = p; }
void Partner::setAddress(const std::string& a) { address = a; }
void Partner::setEmail(const std::string& e) { email = e; }
