#ifndef MODELS_STOCKITEM_H
#define MODELS_STOCKITEM_H

#include <string>
#include "ds/DoublyLinkedList.h"
#include "ds/MinHeap.h"
#include "models/Batch.h"

// Tồn kho của 1 mặt hàng trong 1 kho
class StockItem {
private:
    std::string productId;
    std::string warehouseId;
    DoublyLinkedList<Batch*> batches;               // xem chi tiết từng lô
    MinHeap<Batch*, BatchPtrLess> fefoHeap;         // xuất theo FEFO

public:
    StockItem();
    StockItem(const std::string& productId, const std::string& warehouseId);
    ~StockItem();

    std::string getProductId() const;
    std::string getWarehouseId() const;

    void addBatch(Batch* b);
    int takeFEFO(int qty);          // xuất qty theo FEFO, trả về số lượng thực xuất
    int getTotalQty() const;
    DoublyLinkedList<Batch*>& getBatches();
};

#endif // MODELS_STOCKITEM_H
