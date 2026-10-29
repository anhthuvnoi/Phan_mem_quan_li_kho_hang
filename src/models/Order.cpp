#include "models/Order.h"

Order::Order()
    : orderId(), customerId(), warehouseId(), date(), urgent(false), status(OrderStatus::PENDING), lines() {}

Order::Order(const std::string& orderId, const std::string& customerId,
             const std::string& warehouseId, const Date& date, bool urgent)
    : orderId(orderId), customerId(customerId), warehouseId(warehouseId), date(date),
      urgent(urgent), status(OrderStatus::PENDING), lines() {}

std::string Order::getOrderId() const { return orderId; }
std::string Order::getCustomerId() const { return customerId; }
std::string Order::getWarehouseId() const { return warehouseId; }
Date Order::getDate() const { return date; }
bool Order::isUrgent() const { return urgent; }
OrderStatus Order::getStatus() const { return status; }
void Order::setStatus(OrderStatus s) { status = s; }
void Order::addLine(const ReceiptLine& line) { lines.pushBack(line); }
DoublyLinkedList<ReceiptLine>& Order::getLines() { return lines; }
