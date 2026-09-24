#ifndef MODELS_PRODUCT_H
#define MODELS_PRODUCT_H

#include "models/Entity.h"

class Product : public Entity {
private:
    std::string unit;        // đơn vị tính: hộp, kg, thùng...
    std::string category;
    double salePrice;
    int minStock;            // ngưỡng tồn tối thiểu để cảnh báo

public:
    Product();
    Product(const std::string& id, const std::string& name, const std::string& unit,
            const std::string& category, double salePrice, int minStock);

    std::string getUnit() const;
    std::string getCategory() const;
    double getSalePrice() const;
    int getMinStock() const;
    bool setSalePrice(double p);   // trả false nếu p < 0
    bool setMinStock(int m);

    std::string toString() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string& line) override;
};

#endif // MODELS_PRODUCT_H
