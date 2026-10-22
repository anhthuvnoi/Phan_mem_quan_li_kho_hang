#ifndef MODELS_RECEIPT_H
#define MODELS_RECEIPT_H

#include <string>
#include "models/Date.h"
#include "models/ReceiptLine.h"
#include "ds/DoublyLinkedList.h"

class InventoryService;   // khai báo trước để tránh include vòng

// Lớp trừu tượng: phiếu kho (đa hình qua apply)
class Receipt {
protected:
    std::string id;
    Date date;
    DoublyLinkedList<ReceiptLine> lines;

public:
    Receipt();
    Receipt(const std::string& id, const Date& date);
    virtual ~Receipt();

    std::string getId() const;
    Date getDate() const;
    void addLine(const ReceiptLine& line);
    DoublyLinkedList<ReceiptLine>& getLines();
    double total() const;

    virtual bool apply(InventoryService& inv) = 0;   // tác động lên tồn kho
    virtual std::string getType() const = 0;         // "IMPORT" / "EXPORT" / "TRANSFER"
    virtual std::string toString() const = 0;
    virtual std::string toFileString() const = 0;
};

#endif // MODELS_RECEIPT_H
