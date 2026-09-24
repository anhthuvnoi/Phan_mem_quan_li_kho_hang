#ifndef MODELS_IMPORTRECEIPT_H
#define MODELS_IMPORTRECEIPT_H

#include "models/Receipt.h"

class ImportReceipt : public Receipt {
private:
    std::string supplierId;
    std::string warehouseId;

public:
    ImportReceipt();
    ImportReceipt(const std::string& id, const Date& date,
                  const std::string& supplierId, const std::string& warehouseId);

    std::string getSupplierId() const;
    std::string getWarehouseId() const;

    bool apply(InventoryService& inv) override;   // tạo Batch mới, cộng kho
    std::string getType() const override;
    std::string toString() const override;
    std::string toFileString() const override;
};

#endif // MODELS_IMPORTRECEIPT_H
