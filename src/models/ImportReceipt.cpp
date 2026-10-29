#include "models/ImportReceipt.h"
#include "services/InventoryService.h"
#include <iomanip>
#include <sstream>

// File: dòng đầu  IMPORT|id|date|supplierId|warehouseId|soDong
//       mỗi dòng  L|productId|batchId|qty|unitPrice|mfgDate|expDate

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
    *static_cast<std::string*>(ctx) += "\n  " + l.getProductId() + " (lo " + l.getBatchId() + ") x" +
        std::to_string(l.getQty()) + " @ " + num(l.getUnitPrice()) + " | NSX " + l.getMfgDate().toString() +
        " | HSD " + l.getExpDate().toString();
}
// Lô có mã batchId trong StockItem (nullptr nếu không có)
struct FindBatch { std::string id; Batch* found; };
void findBatch(Batch* const& b, void* ctx) {
    FindBatch* f = static_cast<FindBatch*>(ctx);
    if (f->found == nullptr && b->getBatchId() == f->id) f->found = b;
}
} // namespace

ImportReceipt::ImportReceipt() : Receipt(), supplierId(), warehouseId() {}

ImportReceipt::ImportReceipt(const std::string& id, const Date& date,
                             const std::string& supplierId, const std::string& warehouseId)
    : Receipt(id, date), supplierId(supplierId), warehouseId(warehouseId) {}

std::string ImportReceipt::getSupplierId() const { return supplierId; }
std::string ImportReceipt::getWarehouseId() const { return warehouseId; }
std::string ImportReceipt::getType() const { return "IMPORT"; }

// Kiểm tra TOÀN BỘ phiếu trước; chỉ khi hợp lệ mới tạo lô. Sai -> false, kho không đổi.
bool ImportReceipt::apply(InventoryService& inv) {
    Catalog& cat = inv.getCatalog();
    if (!date.isValid() || lines.empty()) return false;
    Warehouse* wh = cat.findWarehouse(warehouseId);
    if (wh == nullptr || cat.findSupplier(supplierId) == nullptr) return false;

    int n = lines.size();
    for (int i = 0; i < n; i++) {
        ReceiptLine& l = lines.at(i);
        if (cat.findProduct(l.getProductId()) == nullptr) return false;
        if (l.getQty() <= 0 || l.getUnitPrice() < 0) return false;
        if (!l.getMfgDate().isValid() || !l.getExpDate().isValid()) return false;
        if (l.getExpDate() < l.getMfgDate() || date < l.getMfgDate()) return false;

        if (l.getBatchId().empty())                   // tự sinh mã lô: <mã phiếu>-<số dòng>
            l.setBatchInfo(id + "-" + std::to_string(i + 1), l.getMfgDate(), l.getExpDate());

        StockItem* s = wh->getStock(l.getProductId());
        if (s != nullptr) {                           // mã lô đã có trong kho
            FindBatch f{l.getBatchId(), nullptr};
            s->getBatches().forEach(findBatch, &f);
            if (f.found != nullptr) return false;
        }
        for (int j = 0; j < i; j++) {                 // trùng mã lô trong cùng phiếu
            ReceiptLine& o = lines.at(j);
            if (o.getProductId() == l.getProductId() && o.getBatchId() == l.getBatchId()) return false;
        }
    }

    // Mỗi dòng tạo 1 lô mới, giá vốn = đơn giá nhập
    for (int i = 0; i < n; i++) {
        ReceiptLine& l = lines.at(i);
        Batch* b = new Batch(l.getBatchId(), l.getProductId(), warehouseId,
                             l.getMfgDate(), l.getExpDate(), l.getUnitPrice(), l.getQty());
        wh->getOrCreateStock(l.getProductId())->addBatch(b);
    }
    return true;
}

std::string ImportReceipt::toString() const {
    std::string s = "[Phieu nhap] " + id + " | Ngay: " + date.toString() + " | NCC: " + supplierId +
                    " | Kho: " + warehouseId + " | Tong: " + num(total());
    lines.forEach(appendLineText, &s);
    return s;
}

std::string ImportReceipt::toFileString() const {
    std::string s = "IMPORT|" + id + "|" + date.toString() + "|" + supplierId + "|" + warehouseId +
                    "|" + std::to_string(lines.size());
    lines.forEach(appendLineFile, &s);
    return s;
}
