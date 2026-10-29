#include "models/StockItem.h"

// batches  : MỌI lô (kể cả đã xuất hết) — xem chi tiết, ghi file
// fefoHeap : lô HSD sớm nhất ở đỉnh -> xuất trước (FEFO).
//            Lô về 0 (do takeFEFO hoặc Batch::take ở nơi khác) được gỡ khỏi heap khi gặp.

StockItem::StockItem() : productId(), warehouseId(), batches(), fefoHeap() {}

StockItem::StockItem(const std::string& productId, const std::string& warehouseId)
    : productId(productId), warehouseId(warehouseId), batches(), fefoHeap() {}

// StockItem sở hữu các Batch*
StockItem::~StockItem() {
    while (!batches.empty()) {
        delete batches.front();
        batches.popFront();
    }
}

std::string StockItem::getProductId() const { return productId; }
std::string StockItem::getWarehouseId() const { return warehouseId; }

// Nhận quyền sở hữu b. Bỏ qua nếu null hoặc sai mặt hàng (khi đó người gọi tự delete).
void StockItem::addBatch(Batch* b) {
    if (b == nullptr) return;
    if (!productId.empty() && b->getProductId() != productId) return;
    batches.pushBack(b);
    if (b->getQtyRemain() > 0) fefoHeap.push(b);
}

int StockItem::takeFEFO(int qty) {
    int total = 0;
    while (qty > 0 && !fefoHeap.empty()) {
        Batch* b = fefoHeap.top();
        if (b->getQtyRemain() == 0) {        // lô đã hết -> gỡ
            fefoHeap.pop();
            continue;
        }
        int got = b->take(qty);
        total += got;
        qty -= got;
        if (b->getQtyRemain() == 0) fefoHeap.pop();
    }
    return total;
}

namespace {
void sumRemain(Batch* const& b, void* ctx) {
    *static_cast<int*>(ctx) += b->getQtyRemain();
}
} // namespace

int StockItem::getTotalQty() const {
    int total = 0;
    batches.forEach(sumRemain, &total);
    return total;
}

DoublyLinkedList<Batch*>& StockItem::getBatches() { return batches; }
