#include "models/TransferReceipt.h"
#include "services/InventoryService.h"
#include "ds/MinHeap.h"
#include <iomanip>
#include <sstream>

// File: dòng đầu  TRANSFER|id|date|fromWarehouseId|toWarehouseId|soDong
//       mỗi dòng  L|productId|batchId|qty|unitPrice|mfgDate|expDate
//
// Lấy FEFO (bỏ lô hết hạn) ở kho xuất; mỗi phần lấy từ lô X tạo lô "X/<mã phiếu>" ở kho nhập
// GIỮ NGUYÊN NSX, HSD, giá vốn.

namespace {
std::string num(double x) {
    std::ostringstream os;
    os << std::setprecision(15) << x;
    return os.str();
}
void appendLineFile(const ReceiptLine& l, void* ctx) {
    *static_cast<std::string*>(ctx) += "\nL|" + l.getProductId() + "|" + l.getBatchId() + "|" +
        std::to_string(l.getQty()) + "|" + num(l.getUnitPrice()) + "|" +
        l.getMfgDate().toString() + "|" + l.getExpDate().toString();
}
void appendLineText(const ReceiptLine& l, void* ctx) {
    *static_cast<std::string*>(ctx) += "\n  " + l.getProductId() + " x" + std::to_string(l.getQty());
}

struct Sellable { Date today; int qty; MinHeap<Batch*, BatchPtrLess>* heap; };
void collectSellable(Batch* const& b, void* ctx) {
    Sellable* s = static_cast<Sellable*>(ctx);
    if (b->getQtyRemain() > 0 && !b->isExpired(s->today)) {
        s->qty += b->getQtyRemain();
        if (s->heap != nullptr) s->heap->push(b);
    }
}
int sellableQty(StockItem* s, const Date& today) {
    if (s == nullptr) return 0;
    Sellable ctx{today, 0, nullptr};
    s->getBatches().forEach(collectSellable, &ctx);
    return ctx.qty;
}

struct FindBatch { std::string id; Batch* found; };
void findBatch(Batch* const& b, void* ctx) {
    FindBatch* f = static_cast<FindBatch*>(ctx);
    if (f->found == nullptr && b->getBatchId() == f->id) f->found = b;
}
} // namespace

TransferReceipt::TransferReceipt() : Receipt(), fromWarehouseId(), toWarehouseId() {}

TransferReceipt::TransferReceipt(const std::string& id, const Date& date,
                                 const std::string& fromWh, const std::string& toWh)
    : Receipt(id, date), fromWarehouseId(fromWh), toWarehouseId(toWh) {}

std::string TransferReceipt::getFromWarehouseId() const { return fromWarehouseId; }
std::string TransferReceipt::getToWarehouseId() const { return toWarehouseId; }
std::string TransferReceipt::getType() const { return "TRANSFER"; }

bool TransferReceipt::apply(InventoryService& inv) {
    Catalog& cat = inv.getCatalog();
    if (!date.isValid() || lines.empty() || fromWarehouseId == toWarehouseId) return false;
    Warehouse* from = cat.findWarehouse(fromWarehouseId);
    Warehouse* to = cat.findWarehouse(toWarehouseId);
    if (from == nullptr || to == nullptr) return false;

    int n = lines.size();
    for (int i = 0; i < n; i++) {
        ReceiptLine& l = lines.at(i);
        if (cat.findProduct(l.getProductId()) == nullptr || l.getQty() <= 0) return false;
        int need = 0;
        for (int j = 0; j < n; j++)
            if (lines.at(j).getProductId() == l.getProductId()) need += lines.at(j).getQty();
        if (need > sellableQty(from->getStock(l.getProductId()), date)) return false;
    }

    for (int i = 0; i < n; i++) {
        ReceiptLine& l = lines.at(i);
        StockItem* src = from->getStock(l.getProductId());
        StockItem* dst = to->getOrCreateStock(l.getProductId());

        MinHeap<Batch*, BatchPtrLess> heap;            // FEFO trên các lô còn hạn
        Sellable ctx{date, 0, &heap};
        src->getBatches().forEach(collectSellable, &ctx);

        int qty = l.getQty();
        while (qty > 0 && !heap.empty()) {
            Batch* b = heap.top();
            heap.pop();
            int got = b->take(qty);
            qty -= got;
            std::string newId = b->getBatchId() + "/" + id;
            FindBatch f{newId, nullptr};
            dst->getBatches().forEach(findBatch, &f);
            if (f.found != nullptr) {
                f.found->addQty(got);                   // 2 dòng cùng phiếu lấy chung 1 lô
            } else {
                dst->addBatch(new Batch(newId, b->getProductId(), toWarehouseId,
                                        b->getMfgDate(), b->getExpDate(), b->getCostPrice(), got));
            }
        }
    }
    return true;
}

std::string TransferReceipt::toString() const {
    std::string s = "[Phieu dieu chuyen] " + id + " | Ngay: " + date.toString() + " | " +
                    fromWarehouseId + " -> " + toWarehouseId;
    lines.forEach(appendLineText, &s);
    return s;
}

std::string TransferReceipt::toFileString() const {
    std::string s = "TRANSFER|" + id + "|" + date.toString() + "|" + fromWarehouseId + "|" + toWarehouseId +
                    "|" + std::to_string(lines.size());
    lines.forEach(appendLineFile, &s);
    return s;
}
