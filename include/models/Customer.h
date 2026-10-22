#ifndef MODELS_CUSTOMER_H
#define MODELS_CUSTOMER_H

#include "models/Partner.h"

class Customer : public Partner {
private:
    bool vip;

public:
    Customer();
    Customer(const std::string& id, const std::string& name, const std::string& phone,
             const std::string& address, const std::string& email, bool vip);

    bool isVIP() const;
    void setVIP(bool v);

    std::string toString() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string& line) override;
};

#endif // MODELS_CUSTOMER_H
