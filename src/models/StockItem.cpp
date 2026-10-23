#include "models/StockItem.h"
#include <algorithm>
using namespace std;
StockItem::StockItem() : productId(""), warehouseId("") {}
StockItem::StockItem(const string& productId, const string& warehouseId)
: productId(productId), warehouseId(warehouseId) {

}
StockItem::~StockItem() {
    for(int i=0; i<batches.size(); i++){
        Batch* b = batches.at(i);
        delete b;
    }
    batches.clear();
}
string StockItem::getProductId() const {
    return productId;
}
string StockItem::getWarehouseId() const {
    return warehouseId;
}
void StockItem::addBatch(Batch* b) {
    if(!b) return;
    batches.pushBack(b);
    fefoHeap.push(b);
}
int StockItem::takeFEFO(int qty) {
    if (qty <= 0) return 0;

    int totalTaken = 0;
    int remainingToTake = qty;

    while (remainingToTake > 0 && !fefoHeap.empty()) {
        Batch* topBatch = fefoHeap.top();

        if (topBatch->getQtyRemain() <= 0) {
            fefoHeap.pop();
            continue;
        }

        int taken = topBatch->take(remainingToTake);
        totalTaken += taken;
        remainingToTake -= taken;

        if (topBatch->getQtyRemain() == 0) {
            fefoHeap.pop();
        }
    }

    return totalTaken;
}

int StockItem::getTotalQty() const {
    int total = 0;
    for (int i = 0; i < batches.size(); ++i) {
        Batch* b = batches.at(i);
        if (b) {
            total += b->getQtyRemain();
        }
    }
    return total;
}

DoublyLinkedList<Batch*>& StockItem::getBatches() {
    return batches;
}
