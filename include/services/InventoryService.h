#ifndef SERVICES_INVENTORYSERVICE_H
#define SERVICES_INVENTORYSERVICE_H

#include "services/Catalog.h"
#include "services/Result.h"
#include "models/Receipt.h"
#include "ds/DoublyLinkedList.h"
#include "ds/MaxHeap.h"

class ReportService;

// Nhập / xuất / điều chuyển kho
class InventoryService {
private:
    Catalog& catalog;
    ReportService& report;
    DoublyLinkedList<Receipt*> history;    // lịch sử phiếu

public:
    InventoryService(Catalog& catalog, ReportService& report);
    ~InventoryService();

    Catalog& getCatalog();
    ReportService& getReport();

    Result processReceipt(Receipt* r);     // gọi r->apply(*this) (đa hình), lưu history
    DoublyLinkedList<Receipt*>& getHistory();

    // Gợi ý kho nguồn có tồn cao nhất cho mặt hàng (Max-Heap), trả về warehouseId
    std::string suggestSourceWarehouse(const std::string& productId,
                                       const std::string& excludeWarehouseId);
};

#endif // SERVICES_INVENTORYSERVICE_H
