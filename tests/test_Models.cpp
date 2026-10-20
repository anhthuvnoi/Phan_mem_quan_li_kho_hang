#include <cassert>
#include <iostream>
#include <string>
#include "models/Supplier.h"
#include "models/Customer.h"
#include "models/Product.h"
#include "models/Batch.h"
#include "models/ReceiptLine.h"
#include "models/Order.h"
#include "models/TextUtil.h"

int main() {
    // ---------- TextUtil ----------
    std::string p[4];
    assert(TextUtil::split("a|b||d", p, 4) == 4 && p[0] == "a" && p[2] == "" && p[3] == "d");
    assert(TextUtil::split("a|b|c|d|e", p, 4) == 4 && p[3] == "d|e");     // dư -> gộp phần cuối
    assert(TextUtil::split("x\r", p, 4) == 1 && p[0] == "x");             // file Windows (CRLF)
    assert(TextUtil::toInt(" 42 ") == 42 && TextUtil::toInt("4x", -1) == -1 && TextUtil::toInt("") == 0);
    assert(TextUtil::toDouble("32000") == 32000 && TextUtil::toDouble("abc", -1) == -1);
    assert(TextUtil::formatNumber(32000) == "32000" && TextUtil::formatNumber(12.5) == "12.50");
    assert(TextUtil::sanitize("a|b\nc") == "a/b c");

    // ---------- Đa hình qua Entity* ----------
    Entity* list[3];
    list[0] = new Supplier("S001", "Cong ty ABC", "0905000111", "Da Nang", "abc@mail.com", "0401234567");
    list[1] = new Customer("C001", "Nguyen Van A", "0905111222", "Hai Chau, Da Nang", "a@mail.com", true);
    list[2] = new Product("P001", "Sua tuoi Vinamilk 1L", "hop", "Sua", 32000, 20);
    assert(list[0]->toString().find("[NCC]") == 0);
    assert(list[1]->toString().find("VIP") != std::string::npos);
    assert(list[2]->toString().find("32000") != std::string::npos);
    for (int i = 0; i < 3; i++) delete list[i];      // virtual destructor

    // ---------- Supplier: ghi -> đọc ----------
    Supplier s("S001", "Cong ty ABC", "0905000111", "Da Nang", "abc@mail.com", "0401234567");
    assert(s.toFileString() == "S001|Cong ty ABC|0905000111|Da Nang|abc@mail.com|0401234567");
    Supplier s2;
    s2.fromFileString(s.toFileString());
    assert(s2.getId() == "S001" && s2.getTaxCode() == "0401234567" && s2.getEmail() == "abc@mail.com");

    // ---------- Customer ----------
    Customer c;
    c.fromFileString("C001|Nguyen Van A|0905111222|Hai Chau, Da Nang|a@mail.com|1");
    assert(c.isVIP() && c.getAddress() == "Hai Chau, Da Nang" && c.getPhone() == "0905111222");
    c.setVIP(false);
    assert(c.toFileString() == "C001|Nguyen Van A|0905111222|Hai Chau, Da Nang|a@mail.com|0");

    // ---------- Product ----------
    Product pr;
    pr.fromFileString("P001|Sua tuoi Vinamilk 1L|hop|Sua|32000|20");
    assert(pr.getName() == "Sua tuoi Vinamilk 1L" && pr.getSalePrice() == 32000 && pr.getMinStock() == 20);
    assert(!pr.setSalePrice(-1) && pr.getSalePrice() == 32000);
    assert(pr.setSalePrice(35500.5) && !pr.setMinStock(-3));
    Product pr2;
    pr2.fromFileString(pr.toFileString());
    assert(pr2.getSalePrice() == 35500.5 && pr2.getUnit() == "hop");
    Product weird("P9", "Ten|co|gach", "goi", "X", 1, 1);
    assert(weird.toFileString() == "P9|Ten/co/gach|goi|X|1|1");    // không làm hỏng file

    // ---------- Batch ----------
    Batch b("B001", "P001", "W001", Date(1, 9, 2026), Date(1, 10, 2026), 25000, 100);
    assert(b.getQtyRemain() == 100 && b.getQtyIn() == 100);
    assert(b.take(30) == 30 && b.getQtyRemain() == 70);
    assert(b.take(1000) == 70 && b.getQtyRemain() == 0);
    assert(b.take(-5) == 0);
    b.addQty(10);
    assert(b.getQtyRemain() == 10 && b.getQtyIn() == 110);
    assert(!b.isExpired(Date(1, 10, 2026)) && b.isExpired(Date(2, 10, 2026)));
    assert(b.daysToExpire(Date(24, 9, 2026)) == 7 && b.daysToExpire(Date(3, 10, 2026)) == -2);
    Batch b2;
    b2.fromFileString("B001|P001|W001|01/09/2026|01/10/2026|25000|100|100");
    assert(b2.getExpDate() == Date(1, 10, 2026) && b2.getCostPrice() == 25000 && b2.getQtyRemain() == 100);
    assert(b2.toFileString() == "B001|P001|W001|01/09/2026|01/10/2026|25000|100|100");
    Batch early("B0", "P001", "W001", Date(1, 1, 2026), Date(1, 3, 2026), 1, 1);
    assert(early < b2 && !(b2 < early));
    BatchPtrLess less;
    assert(less(&early, &b2));

    // ---------- ReceiptLine ----------
    ReceiptLine l("P001", 3, 32000);
    assert(l.lineTotal() == 96000);
    l.setBatchInfo("B9", Date(1, 9, 2026), Date(1, 12, 2026));
    ReceiptLine l2;
    l2.fromFileString(l.toFileString());
    assert(l2 == l && l2.getBatchId() == "B9" && l2.getExpDate() == Date(1, 12, 2026));

    // ---------- Order ----------
    Order o("DH001", "C001", "W001", Date(24, 9, 2026), true);
    o.addLine(ReceiptLine("P001", 2, 32000));
    o.addLine(ReceiptLine("P002", 10, 4500));
    assert(o.isUrgent() && o.getStatus() == OrderStatus::PENDING && o.getLines().size() == 2);
    assert(o.total() == 109000);
    o.setStatus(OrderStatus::DONE);
    assert(Order::statusToString(o.getStatus()) == "DONE");
    assert(Order::statusFromString("CANCELLED") == OrderStatus::CANCELLED);
    assert(o.toString().find("KHAN") != std::string::npos);

    std::cout << "test_Models: OK\n";
    return 0;
}
