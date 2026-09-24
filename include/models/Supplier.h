#ifndef MODELS_SUPPLIER_H
#define MODELS_SUPPLIER_H

#include "models/Partner.h"

class Supplier : public Partner {
private:
    std::string taxCode;

public:
    Supplier();
    Supplier(const std::string& id, const std::string& name, const std::string& phone,
             const std::string& address, const std::string& email, const std::string& taxCode);

    std::string getTaxCode() const;
    void setTaxCode(const std::string& t);

    std::string toString() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string& line) override;
};

#endif // MODELS_SUPPLIER_H
