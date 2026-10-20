#include <cassert>
#include <iostream>
#include "models/StockItem.h"
#include "models/Warehouse.h"

static Batch* mk(const char* id, int expDay, int qty) {
    return new Batch(id, "P001", "W001", Date(1, 1, 2026), Date(expDay, 10, 2026), 1000, qty);
}

int main() {
    // ---------- StockItem: FEFO ----------
    StockItem s("P001", "W001");
    s.addBatch(mk("L20", 20, 10));      // HSD 20/10
    s.addBatch(mk("L05", 5, 4));        // HSD 05/10 -> xuất trước
    s.addBatch(mk("L12", 12, 6));
    s.addBatch(nullptr);                // bỏ qua
    Batch* other = new Batch("X", "P999", "W001", Date(1, 1, 2026), Date(1, 1, 2027), 1, 5);
    s.addBatch(other);                  // sai mặt hàng -> bỏ qua
    delete other;
    assert(s.getTotalQty() == 20 && s.getBatches().size() == 3);
    assert(s.peekFEFO()->getBatchId() == "L05");

    assert(s.takeFEFO(5) == 5);                         // hết L05 (4) + 1 của L12
    assert(s.findBatch("L05")->getQtyRemain() == 0);
    assert(s.findBatch("L12")->getQtyRemain() == 5);
    assert(s.peekFEFO()->getBatchId() == "L12");
    assert(s.getTotalQty() == 15 && s.getBatches().size() == 3);   // lô hết vẫn giữ lịch sử

    // ---------- takeFEFODetail ----------
    Batch* from[5];
    int amt[5], n = 0;
    assert(s.takeFEFODetail(8, from, amt, 5, n) == 8);
    assert(n == 2 && from[0]->getBatchId() == "L12" && amt[0] == 5 && from[1]->getBatchId() == "L20" && amt[1] == 3);
    assert(s.getTotalQty() == 7);

    // lấy nhiều hơn tồn -> chỉ lấy được phần còn lại
    assert(s.takeFEFO(100) == 7 && s.getTotalQty() == 0 && s.peekFEFO() == nullptr);
    assert(s.takeFEFO(1) == 0);

    // restock lô đã hết -> xuất lại được
    s.restock(s.findBatch("L05"), 3);
    assert(s.getTotalQty() == 3 && s.peekFEFO()->getBatchId() == "L05");

    // ---------- Bỏ qua lô hết hạn ----------
    StockItem e("P001", "W001");
    e.addBatch(mk("OLD", 1, 5));        // hết hạn sau 01/10
    e.addBatch(mk("NEW", 30, 5));
    Date today(10, 10, 2026);
    assert(e.getTotalQty() == 10 && e.getSellableQty(today) == 5);
    assert(e.takeFEFO(3, today) == 3);
    assert(e.findBatch("OLD")->getQtyRemain() == 5 && e.findBatch("NEW")->getQtyRemain() == 2);
    assert(e.takeFEFO(10, today) == 2);                 // không đụng lô hết hạn
    assert(e.getTotalQty() == 5);                       // lô hết hạn vẫn còn để báo huỷ

    // ---------- Warehouse ----------
    Warehouse w("W001", "Kho Lien Chieu", "Lien Chieu, Da Nang");
    assert(w.getStock("P001") == nullptr && w.getQty("P001") == 0);
    StockItem* si = w.getOrCreateStock("P001");
    assert(si == w.getOrCreateStock("P001"));           // không tạo trùng
    si->addBatch(mk("A", 10, 7));
    w.getOrCreateStock("P002")->addBatch(new Batch("B", "P002", "W001", Date(1, 1, 2026), Date(1, 1, 2027), 1, 3));
    assert(w.getQty("P001") == 7 && w.getQty("P002") == 3 && w.getTotalQty() == 10);
    assert(w.getStockTable().size() == 2);
    assert(w.toFileString() == "W001|Kho Lien Chieu|Lien Chieu, Da Nang");
    Warehouse w2;
    w2.fromFileString("W002|Kho Hai Chau|Hai Chau, Da Nang");
    assert(w2.getId() == "W002" && w2.getAddress() == "Hai Chau, Da Nang");
    assert(w.toString().find("Tong ton: 10") != std::string::npos);

    std::cout << "test_StockItem: OK\n";
    return 0;
}
