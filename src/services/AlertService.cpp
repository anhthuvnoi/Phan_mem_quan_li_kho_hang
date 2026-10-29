#include "services/AlertService.h"

AlertService::AlertService(Catalog& catalog) : catalog(catalog) {}

namespace {
struct NearCtx {
    const Catalog* cat;
    Date today;
    int days;
    MinHeap<Batch*, BatchPtrLess>* heap;
    Warehouse* wh;
};
// Lô còn hàng có HSD trong vòng `days` ngày (gồm cả lô đã hết hạn — cần xử lý gấp nhất)
void checkBatch(Batch* const& b, void* ctx) {
    NearCtx* c = static_cast<NearCtx*>(ctx);
    if (b->getQtyRemain() > 0 && b->daysToExpire(c->today) <= c->days) c->heap->push(b);
}
void checkProductInWarehouse(const std::string& productId, Product* const&, void* ctx) {
    NearCtx* c = static_cast<NearCtx*>(ctx);
    StockItem* s = c->wh->getStock(productId);
    if (s != nullptr) s->getBatches().forEach(checkBatch, ctx);
}
void checkWarehouse(const std::string&, Warehouse* const& w, void* ctx) {
    NearCtx* c = static_cast<NearCtx*>(ctx);
    c->wh = w;
    c->cat->getProducts().forEach(checkProductInWarehouse, ctx);
}
} // namespace

// Min-Heap theo HSD -> kết quả sắp HSD tăng dần (sắp hết hạn nhất đứng đầu)
int AlertService::nearExpiry(const Date& today, int days, Batch* results[], int maxResults) {
    MinHeap<Batch*, BatchPtrLess> heap;
    NearCtx ctx{&catalog, today, days, &heap, nullptr};
    catalog.getWarehouses().forEach(checkWarehouse, &ctx);
    int n = 0;
    while (!heap.empty() && n < maxResults) {
        results[n++] = heap.top();
        heap.pop();
    }
    return n;
}

namespace {
struct LowCtx {
    const Catalog* cat;
    std::string* ids;
    int max;
    int n;
};
struct QtyCtx { std::string productId; int total; };
void sumQty(const std::string&, Warehouse* const& w, void* ctx) {
    QtyCtx* q = static_cast<QtyCtx*>(ctx);
    q->total += w->getQty(q->productId);
}
void checkLow(const std::string& id, Product* const& p, void* ctx) {
    LowCtx* c = static_cast<LowCtx*>(ctx);
    if (c->n >= c->max) return;
    QtyCtx q{id, 0};
    c->cat->getWarehouses().forEach(sumQty, &q);
    if (q.total < p->getMinStock()) c->ids[c->n++] = id;
}
} // namespace

// Mặt hàng có tổng tồn (mọi kho) < minStock
int AlertService::lowStock(std::string productIds[], int maxResults) {
    if (maxResults <= 0) return 0;
    LowCtx ctx{&catalog, productIds, maxResults, 0};
    catalog.getProducts().forEach(checkLow, &ctx);
    return ctx.n;
}
