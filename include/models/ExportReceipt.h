#ifndef MODELS_EXPORTRECEIPT_H
#define MODELS_EXPORTRECEIPT_H

#include "models/Receipt.h"

class ExportReceipt : public Receipt {
private:
    std::string customerId;
    std::string warehouseId;

public:
    ExportReceipt();
    ExportReceipt(const std::string& id, const Date& date,
                  const std::string& customerId, const std::string& warehouseId);

    std::string getCustomerId() const;
    std::string getWarehouseId() const;

    bool apply(InventoryService& inv) override;   // trừ kho theo FEFO, ghi doanh thu
    std::string getType() const override;
    std::string toString() const override;
    std::string toFileString() const override;
};

#endif // MODELS_EXPORTRECEIPT_H
