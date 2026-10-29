#include "models/ExportReceipt.h"
#include "services/InventoryService.h"
#include "services/ReportService.h"
#include "ds/MinHeap.h"
#include <iomanip>
#include <sstream>

// File: dòng đầu  EXPORT|id|date|customerId|warehouseId|soDong
//       mỗi dòng  L|productId|batchId|qty|unitPrice|mfgDate|expDate
//
// Xuất theo FEFO nhưng KHÔNG xuất lô đã hết hạn: dựng 1 MinHeap tạm chỉ gồm các lô
// còn hạn và còn hàng rồi lấy dần từ đỉnh (lô hết hạn để lại cho AlertService báo huỷ).

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
    *static_cast<std::string*>(ctx) += "\n  " + l.getProductId() + " x" + std::to_string(l.getQty()) +
        " @ " + num(l.getUnitPrice()) + " = " + num(l.lineTotal());
}

struct Sellable { Date today; int qty; MinHeap<Batch*, BatchPtrLess>* heap; };
void collectSellable(Batch* const& b, void* ctx) {
    Sellable* s = static_cast<Sellable*>(ctx);
    if (b->getQtyRemain() > 0 && !b->isExpired(s->today)) {
        s->qty += b->getQtyRemain();
        if (s->heap != nullptr) s->heap->push(b);
    }
}

// Tồn còn hạn của 1 mặt hàng trong kho
int sellableQty(StockItem* s, const Date& today) {
    if (s == nullptr) return 0;
    Sellable ctx{today, 0, nullptr};
    s->getBatches().forEach(collectSellable, &ctx);
    return ctx.qty;
}

// Lấy qty theo FEFO, bỏ qua lô hết hạn
void takeSellable(StockItem* s, int qty, const Date& today) {
    MinHeap<Batch*, BatchPtrLess> heap;
    Sellable ctx{today, 0, &heap};
    s->getBatches().forEach(collectSellable, &ctx);
    while (qty > 0 && !heap.empty()) {
        qty -= heap.top()->take(qty);
        heap.pop();
    }
}
} // namespace

ExportReceipt::ExportReceipt() : Receipt(), customerId(), warehouseId() {}

ExportReceipt::ExportReceipt(const std::string& id, const Date& date,
                             const std::string& customerId, const std::string& warehouseId)
    : Receipt(id, date), customerId(customerId), warehouseId(warehouseId) {}

std::string ExportReceipt::getCustomerId() const { return customerId; }
std::string ExportReceipt::getWarehouseId() const { return warehouseId; }
std::string ExportReceipt::getType() const { return "EXPORT"; }

bool ExportReceipt::apply(InventoryService& inv) {
    Catalog& cat = inv.getCatalog();
    if (!date.isValid() || lines.empty()) return false;
    Warehouse* wh = cat.findWarehouse(warehouseId);
    if (wh == nullptr || cat.findCustomer(customerId) == nullptr) return false;

    // Kiểm tra: mặt hàng tồn tại, đủ hàng CÒN HẠN (cộng dồn các dòng cùng mặt hàng)
    int n = lines.size();
    for (int i = 0; i < n; i++) {
        ReceiptLine& l = lines.at(i);
        if (cat.findProduct(l.getProductId()) == nullptr) return false;
        if (l.getQty() <= 0 || l.getUnitPrice() < 0) return false;
        int need = 0;
        for (int j = 0; j < n; j++)
            if (lines.at(j).getProductId() == l.getProductId()) need += lines.at(j).getQty();
        if (need > sellableQty(wh->getStock(l.getProductId()), date)) return false;
    }

    // Trừ kho và ghi doanh thu theo ngày lập phiếu
    for (int i = 0; i < n; i++) {
        ReceiptLine& l = lines.at(i);
        takeSellable(wh->getStock(l.getProductId()), l.getQty(), date);
    }
    inv.getReport().addRevenue(date, total());
    return true;
}

std::string ExportReceipt::toString() const {
    std::string s = "[Phieu xuat] " + id + " | Ngay: " + date.toString() + " | KH: " + customerId +
                    " | Kho: " + warehouseId + " | Tong: " + num(total());
    lines.forEach(appendLineText, &s);
    return s;
}

std::string ExportReceipt::toFileString() const {
    std::string s = "EXPORT|" + id + "|" + date.toString() + "|" + customerId + "|" + warehouseId +
                    "|" + std::to_string(lines.size());
    lines.forEach(appendLineFile, &s);
    return s;
}
