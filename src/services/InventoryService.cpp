#include "services/InventoryService.h"
#include "services/ReportService.h"
#include <utility>

InventoryService::InventoryService(Catalog& catalog, ReportService& report)
    : catalog(catalog), report(report), history() {}

// InventoryService sở hữu các phiếu đã xử lý thành công
InventoryService::~InventoryService() {
    while (!history.empty()) {
        delete history.front();
        history.popFront();
    }
}

Catalog& InventoryService::getCatalog() { return catalog; }
ReportService& InventoryService::getReport() { return report; }
DoublyLinkedList<Receipt*>& InventoryService::getHistory() { return history; }

namespace {
struct FindReceipt { std::string id; bool found; };
void matchReceipt(Receipt* const& r, void* ctx) {
    FindReceipt* f = static_cast<FindReceipt*>(ctx);
    if (r->getId() == f->id) f->found = true;
}
} // namespace

// Thành công: phiếu được lưu vào history (InventoryService giữ và delete sau).
// Thất bại : phiếu KHÔNG được lưu, tồn kho không đổi -> người gọi tự delete r.
Result InventoryService::processReceipt(Receipt* r) {
    if (r == nullptr) return Result(false, "Phieu rong");
    if (r->getId().empty()) return Result(false, "Ma phieu khong duoc de trong");
    FindReceipt f{r->getId(), false};
    history.forEach(matchReceipt, &f);
    if (f.found) return Result(false, "Ma phieu " + r->getId() + " da ton tai");

    if (!r->apply(*this)) {                                  // đa hình: nhập / xuất / điều chuyển
        return Result(false, "Phieu " + r->getType() + " " + r->getId() +
                             " khong hop le: kiem tra ma kho/doi tac/mat hang, so luong, ngay, "
                             "NSX/HSD hoac ton kho con han");
    }
    history.pushBack(r);
    return Result(true, "Da xu ly phieu " + r->getType() + " " + r->getId());
}

namespace {
struct Suggest {
    std::string productId;
    std::string exclude;
    MaxHeap<std::pair<int, std::string>>* heap;
};
void pushWarehouse(const std::string& id, Warehouse* const& w, void* ctx) {
    Suggest* s = static_cast<Suggest*>(ctx);
    if (id == s->exclude) return;
    int qty = w->getQty(s->productId);
    if (qty > 0) s->heap->push(std::make_pair(qty, id));
}
} // namespace

// Max-Heap theo (tồn, mã kho) -> đỉnh là kho tồn nhiều nhất. "" nếu không kho nào có hàng.
std::string InventoryService::suggestSourceWarehouse(const std::string& productId,
                                                     const std::string& excludeWarehouseId) {
    MaxHeap<std::pair<int, std::string>> heap;
    Suggest s{productId, excludeWarehouseId, &heap};
    catalog.getWarehouses().forEach(pushWarehouse, &s);
    return heap.empty() ? "" : heap.top().second;
}
