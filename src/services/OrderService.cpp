#include "services/OrderService.h"
#include "services/InventoryService.h"
#include "models/ExportReceipt.h"

// OrderService SỞ HỮU các Order* trong hàng đợi. processNext() soạn xong thì delete đơn.

OrderService::OrderService(InventoryService& inv) : queue(), inventory(inv) {}

OrderService::~OrderService() {
    while (!queue.empty()) {
        delete queue.front();
        queue.popFront();
    }
}

// Đơn khẩn / VIP lên đầu hàng đợi, đơn thường xếp cuối
void OrderService::enqueue(Order* o) {
    if (o == nullptr) return;
    o->setStatus(OrderStatus::PENDING);
    if (o->isUrgent()) queue.pushFront(o);
    else queue.pushBack(o);
}

Order* OrderService::peekNext() {
    return queue.empty() ? nullptr : queue.front();
}

namespace {
void copyLine(const ReceiptLine& l, void* ctx) {
    static_cast<ExportReceipt*>(ctx)->addLine(l);
}
} // namespace

// Lấy đơn đầu hàng đợi -> tạo phiếu xuất "PX-<mã đơn>" -> xử lý qua InventoryService.
// Thành công: DONE; thất bại (thiếu hàng...): CANCELLED. Đơn được bỏ khỏi hàng đợi và delete.
Result OrderService::processNext() {
    if (queue.empty()) return Result(false, "Khong co don nao dang cho");
    Order* o = queue.front();
    queue.popFront();
    o->setStatus(OrderStatus::PICKING);

    ExportReceipt* r = new ExportReceipt("PX-" + o->getOrderId(), o->getDate(),
                                         o->getCustomerId(), o->getWarehouseId());
    o->getLines().forEach(copyLine, r);
    Result res = inventory.processReceipt(r);

    std::string orderId = o->getOrderId();
    if (res.ok) {
        o->setStatus(OrderStatus::DONE);
        res.message = "Don " + orderId + " da soan xong. " + res.message;
    } else {
        delete r;                                   // phiếu không được lưu
        o->setStatus(OrderStatus::CANCELLED);
        res.message = "Don " + orderId + " bi huy. " + res.message;
    }
    delete o;
    return res;
}

int OrderService::pendingCount() const {
    return queue.size();
}
