#ifndef SERVICES_CATALOG_H
#define SERVICES_CATALOG_H

#include "ds/HashTable.h"
#include "models/Product.h"
#include "models/Supplier.h"
#include "models/Customer.h"
#include "models/Warehouse.h"
#include "services/Result.h"

// Quản lý danh mục (dữ liệu nền) — Hash Table
class Catalog {
private:
    HashTable<std::string, Product*> products;
    HashTable<std::string, Supplier*> suppliers;
    HashTable<std::string, Customer*> customers;
    HashTable<std::string, Warehouse*> warehouses;

public:
    Catalog();
    ~Catalog();

    Result addProduct(Product* p);
    Result updateProduct(const Product& p);
    Result removeProduct(const std::string& id);
    Product* findProduct(const std::string& id);

    Result addSupplier(Supplier* s);
    Result updateSupplier(const Supplier& s);
    Result removeSupplier(const std::string& id);
    Supplier* findSupplier(const std::string& id);

    Result addCustomer(Customer* c);
    Result updateCustomer(const Customer& c);
    Result removeCustomer(const std::string& id);
    Customer* findCustomer(const std::string& id);

    Result addWarehouse(Warehouse* w);
    Result updateWarehouse(const Warehouse& w);
    Result removeWarehouse(const std::string& id);
    Warehouse* findWarehouse(const std::string& id);

    const HashTable<std::string, Product*>& getProducts() const;      // để duyệt (forEach)
    const HashTable<std::string, Supplier*>& getSuppliers() const;
    const HashTable<std::string, Customer*>& getCustomers() const;
    const HashTable<std::string, Warehouse*>& getWarehouses() const;
};

#endif // SERVICES_CATALOG_H
