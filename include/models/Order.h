#ifndef MODELS_ORDER_H
#define MODELS_ORDER_H

#include <string>
#include "models/Date.h"
#include "models/ReceiptLine.h"
#include "ds/DoublyLinkedList.h"

enum class OrderStatus { PENDING, PICKING, DONE, CANCELLED };

// Đơn hàng chờ soạn (đưa vào Deque)
class Order {
private:
    std::string orderId;
    std::string customerId;
    std::string warehouseId;
    Date date;
    bool urgent;             // đơn hoả tốc / VIP -> đẩy lên đầu hàng đợi
    OrderStatus status;
    DoublyLinkedList<ReceiptLine> lines;

public:
    Order();
    Order(const std::string& orderId, const std::string& customerId,
          const std::string& warehouseId, const Date& date, bool urgent);

    std::string getOrderId() const;
    std::string getCustomerId() const;
    std::string getWarehouseId() const;
    Date getDate() const;
    bool isUrgent() const;
    OrderStatus getStatus() const;
    void setStatus(OrderStatus s);
    void addLine(const ReceiptLine& line);
    DoublyLinkedList<ReceiptLine>& getLines();
};

#endif // MODELS_ORDER_H
