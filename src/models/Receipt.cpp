#include "models/Receipt.h"

Receipt::Receipt() : id(), date(), lines() {}
Receipt::Receipt(const std::string& id, const Date& date) : id(id), date(date), lines() {}
Receipt::~Receipt() {}

std::string Receipt::getId() const { return id; }
Date Receipt::getDate() const { return date; }
void Receipt::addLine(const ReceiptLine& line) { lines.pushBack(line); }
DoublyLinkedList<ReceiptLine>& Receipt::getLines() { return lines; }

namespace {
void addLineTotal(const ReceiptLine& l, void* ctx) {
    *static_cast<double*>(ctx) += l.lineTotal();
}
} // namespace

double Receipt::total() const {
    double sum = 0;
    lines.forEach(addLineTotal, &sum);
    return sum;
}
