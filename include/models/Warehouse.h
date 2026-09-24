#ifndef MODELS_WAREHOUSE_H
#define MODELS_WAREHOUSE_H

#include "models/Entity.h"
#include "models/StockItem.h"
#include "ds/HashTable.h"

class Warehouse : public Entity {
private:
    std::string address;
    HashTable<std::string, StockItem*> stock;   // productId -> tồn kho

public:
    Warehouse();
    Warehouse(const std::string& id, const std::string& name, const std::string& address);
    ~Warehouse();

    std::string getAddress() const;
    void setAddress(const std::string& a);

    StockItem* getStock(const std::string& productId);
    StockItem* getOrCreateStock(const std::string& productId);
    int getQty(const std::string& productId);

    std::string toString() const override;
    std::string toFileString() const override;
    void fromFileString(const std::string& line) override;
};

#endif // MODELS_WAREHOUSE_H
