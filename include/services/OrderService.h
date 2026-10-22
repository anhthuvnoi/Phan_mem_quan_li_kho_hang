#ifndef SERVICES_ORDERSERVICE_H
#define SERVICES_ORDERSERVICE_H

#include "ds/Deque.h"
#include "models/Order.h"
#include "services/Result.h"

class InventoryService;

// Hàng đợi đơn chờ soạn — Deque
class OrderService {
private:
    Deque<Order*> queue;
    InventoryService& inventory;

public:
    OrderService(InventoryService& inv);
    ~OrderService();

    void enqueue(Order* o);      // urgent -> pushFront, thường -> pushBack
    Order* peekNext();
    Result processNext();        // soạn đơn đầu hàng đợi -> tạo ExportReceipt
    int pendingCount() const;
};

#endif // SERVICES_ORDERSERVICE_H
