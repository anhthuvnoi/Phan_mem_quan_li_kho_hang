#ifndef MODELS_TRANSFERRECEIPT_H
#define MODELS_TRANSFERRECEIPT_H

#include "models/Receipt.h"

class TransferReceipt : public Receipt {
private:
    std::string fromWarehouseId;
    std::string toWarehouseId;

public:
    TransferReceipt();
    TransferReceipt(const std::string& id, const Date& date,
                    const std::string& fromWh, const std::string& toWh);

    std::string getFromWarehouseId() const;
    std::string getToWarehouseId() const;

    bool apply(InventoryService& inv) override;   // trừ kho xuất (FEFO), cộng kho nhập (giữ HSD)
    std::string getType() const override;
    std::string toString() const override;
    std::string toFileString() const override;
};

#endif // MODELS_TRANSFERRECEIPT_H
