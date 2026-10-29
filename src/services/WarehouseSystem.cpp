#include "services/WarehouseSystem.h"

// Thứ tự khởi tạo đúng thứ tự khai báo thành viên trong WarehouseSystem.h:
// catalog -> report -> inventory -> orders -> search -> alerts -> files -> logger
WarehouseSystem::WarehouseSystem()
    : catalog(), report(), inventory(catalog, report), orders(inventory), search(),
      alerts(catalog), files("data/"), logger("data/log.txt") {}

Catalog& WarehouseSystem::getCatalog() { return catalog; }
InventoryService& WarehouseSystem::getInventory() { return inventory; }
OrderService& WarehouseSystem::getOrders() { return orders; }
SearchService& WarehouseSystem::getSearch() { return search; }
AlertService& WarehouseSystem::getAlerts() { return alerts; }
ReportService& WarehouseSystem::getReport() { return report; }

namespace {
struct IndexCtx { const Catalog* cat; SearchService* search; };
struct QtyCtx { std::string productId; int total; };
void sumQty(const std::string&, Warehouse* const& w, void* ctx) {
    QtyCtx* q = static_cast<QtyCtx*>(ctx);
    q->total += w->getQty(q->productId);
}
void indexOne(const std::string& id, Product* const& p, void* ctx) {
    IndexCtx* c = static_cast<IndexCtx*>(ctx);
    QtyCtx q{id, 0};
    c->cat->getWarehouses().forEach(sumQty, &q);
    c->search->indexProduct(p, q.total);
}
} // namespace

// Nạp dữ liệu rồi lập chỉ mục tìm kiếm (Trie tên, BST giá, BST tồn)
Result WarehouseSystem::load() {
    Result r = files.loadAll(catalog, inventory);
    if (r.ok) {
        IndexCtx ctx{&catalog, &search};
        catalog.getProducts().forEach(indexOne, &ctx);
    }
    logger.log("LOAD", r.message);
    return r;
}

Result WarehouseSystem::save() {
    Result r = files.saveAll(catalog, inventory);
    logger.log("SAVE", r.message);
    return r;
}
