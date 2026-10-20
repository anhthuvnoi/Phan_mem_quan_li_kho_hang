#include <cassert>
#include <iostream>
#include "services/Catalog.h"
#include "services/InventoryService.h"
#include "services/ReportService.h"
#include "models/ImportReceipt.h"
#include "models/ExportReceipt.h"
#include "models/TransferReceipt.h"

static ReceiptLine importLine(const char* pid, int qty, double cost, const Date& exp, const char* batch = "") {
    ReceiptLine l(pid, qty, cost);
    l.setBatchInfo(batch, Date(1, 9, 2026), exp);
    return l;
}

int main() {
    Catalog cat;
    ReportService report;
    InventoryService inv(cat, report);

    // ---------- Catalog ----------
    assert(cat.addProduct(new Product("P001", "Sua tuoi", "hop", "Sua", 32000, 20)).ok);
    assert(cat.addProduct(new Product("P002", "Mi Hao Hao", "goi", "Kho", 4500, 100)).ok);
    Product* dup = new Product("P001", "Trung", "hop", "Sua", 1, 1);
    assert(!cat.addProduct(dup).ok);                 // trùng mã -> người gọi giữ con trỏ
    delete dup;
    assert(!cat.addProduct(nullptr).ok);
    Product* noId = new Product("", "X", "", "", 0, 0);
    assert(!cat.addProduct(noId).ok);
    delete noId;
    assert(cat.addSupplier(new Supplier("S001", "ABC", "", "", "", "")).ok);
    assert(cat.addCustomer(new Customer("C001", "A", "", "", "", true)).ok);
    assert(cat.addWarehouse(new Warehouse("W001", "Lien Chieu", "")).ok);
    assert(cat.addWarehouse(new Warehouse("W002", "Hai Chau", "")).ok);
    assert(cat.addWarehouse(new Warehouse("W003", "Son Tra", "")).ok);
    assert(cat.getProducts().size() == 2 && cat.getWarehouses().size() == 3);

    assert(cat.updateProduct(Product("P001", "Sua tuoi 1L", "hop", "Sua", 33000, 20)).ok);
    assert(cat.findProduct("P001")->getName() == "Sua tuoi 1L" && cat.findProduct("P001")->getSalePrice() == 33000);
    assert(!cat.updateProduct(Product("P404", "", "", "", 0, 0)).ok);
    assert(cat.updateWarehouse(Warehouse("W001", "Kho Lien Chieu", "Da Nang")).ok);
    assert(cat.findWarehouse("W001")->getName() == "Kho Lien Chieu");

    // ---------- Phiếu nhập ----------
    ImportReceipt* pn = new ImportReceipt("PN0001", Date(10, 9, 2026), "S001", "W001");
    pn->addLine(importLine("P001", 50, 25000, Date(30, 9, 2026), "B1"));    // hạn sớm
    pn->addLine(importLine("P001", 100, 24000, Date(31, 12, 2026)));        // mã lô tự sinh
    pn->addLine(importLine("P002", 200, 3000, Date(1, 6, 2027)));
    assert(inv.processReceipt(pn).ok);
    Warehouse* w1 = cat.findWarehouse("W001");
    assert(w1->getQty("P001") == 150 && w1->getQty("P002") == 200);
    assert(w1->getStock("P001")->findBatch("PN0001-2") != nullptr);         // mã lô tự sinh
    assert(cat.totalQtyOf("P001") == 150);
    assert(inv.getHistory().size() == 1 && inv.findReceipt("PN0001") == pn);

    // Phiếu lỗi -> không thay đổi gì, có lý do
    ImportReceipt* bad = new ImportReceipt("PN0002", Date(10, 9, 2026), "S001", "W001");
    bad->addLine(importLine("P001", 10, 1, Date(30, 9, 2026)));
    bad->addLine(importLine("P404", 10, 1, Date(30, 9, 2026)));             // mặt hàng không có
    Result r = inv.processReceipt(bad);                                      // bad đã bị delete
    assert(!r.ok && r.message.find("P404") != std::string::npos);
    assert(w1->getQty("P001") == 150 && inv.getHistory().size() == 1);       // dòng 1 KHÔNG được nhập

    ImportReceipt* dupId = new ImportReceipt("PN0001", Date(10, 9, 2026), "S001", "W001");
    dupId->addLine(importLine("P001", 1, 1, Date(30, 9, 2026)));
    assert(!inv.processReceipt(dupId).ok);                                   // trùng mã phiếu

    ImportReceipt* badDate = new ImportReceipt("PN0003", Date(10, 9, 2026), "S001", "W001");
    badDate->addLine(importLine("P001", 1, 1, Date(1, 8, 2026)));           // HSD trước NSX
    assert(!inv.processReceipt(badDate).ok);

    ImportReceipt* dupBatch = new ImportReceipt("PN0004", Date(10, 9, 2026), "S001", "W001");
    dupBatch->addLine(importLine("P001", 1, 1, Date(30, 9, 2026), "B1"));   // lô B1 đã có
    assert(!inv.processReceipt(dupBatch).ok);
    assert(!inv.processReceipt(new ImportReceipt("PN0005", Date(10, 9, 2026), "S404", "W001")).ok);

    // ---------- Phiếu xuất (FEFO + doanh thu) ----------
    ExportReceipt* px = new ExportReceipt("PX0001", Date(15, 9, 2026), "C001", "W001");
    px->addLine(ReceiptLine("P001", 30, 33000));
    px->addLine(ReceiptLine("P001", 30, 33000));      // cùng mặt hàng, 2 dòng -> cần 60
    assert(inv.processReceipt(px).ok);
    StockItem* s1 = w1->getStock("P001");
    assert(s1->findBatch("B1")->getQtyRemain() == 0);                       // lô hạn sớm xuất trước
    assert(s1->findBatch("PN0001-2")->getQtyRemain() == 90);
    assert(report.revenueBetween(Date(15, 9, 2026), Date(15, 9, 2026)) == 60 * 33000);

    ExportReceipt* tooMuch = new ExportReceipt("PX0002", Date(16, 9, 2026), "C001", "W001");
    tooMuch->addLine(ReceiptLine("P001", 50, 1));
    tooMuch->addLine(ReceiptLine("P001", 50, 1));     // tổng 100 > 90
    r = inv.processReceipt(tooMuch);
    assert(!r.ok && r.message.find("khong du") != std::string::npos);
    assert(w1->getQty("P001") == 90);

    // ---------- Điều chuyển (giữ HSD, gợi ý kho nguồn) ----------
    assert(inv.suggestSourceWarehouse("P001", "W002") == "W001");
    assert(inv.suggestSourceWarehouse("P001", "W001") == "");               // không kho khác có hàng
    TransferReceipt* dc = new TransferReceipt("DC0001", Date(20, 9, 2026), "W001", "W002");
    dc->addLine(ReceiptLine("P001", 40, 0));
    assert(inv.processReceipt(dc).ok);
    Warehouse* w2 = cat.findWarehouse("W002");
    assert(w1->getQty("P001") == 50 && w2->getQty("P001") == 40);
    Batch* moved = w2->getStock("P001")->findBatch("PN0001-2");
    assert(moved && moved->getExpDate() == Date(31, 12, 2026) && moved->getCostPrice() == 24000);

    // chuyển thêm lần nữa -> cộng vào cùng lô, không tạo lô trùng
    TransferReceipt* dc2 = new TransferReceipt("DC0002", Date(21, 9, 2026), "W001", "W002");
    dc2->addLine(ReceiptLine("P001", 10, 0));
    assert(inv.processReceipt(dc2).ok);
    assert(w2->getQty("P001") == 50 && w2->getStock("P001")->getBatches().size() == 1);
    assert(inv.suggestSourceWarehouse("P001", "W003") == "W001" || inv.suggestSourceWarehouse("P001", "W003") == "W002");

    TransferReceipt* same = new TransferReceipt("DC0003", Date(21, 9, 2026), "W001", "W001");
    same->addLine(ReceiptLine("P001", 1, 0));
    assert(!inv.processReceipt(same).ok);

    // ---------- Không xuất hàng hết hạn ----------
    ImportReceipt* pn2 = new ImportReceipt("PN0010", Date(1, 10, 2026), "S001", "W003");
    pn2->addLine(importLine("P002", 5, 1, Date(5, 10, 2026)));
    pn2->addLine(importLine("P002", 5, 1, Date(1, 12, 2026)));
    assert(inv.processReceipt(pn2).ok);
    ExportReceipt* late = new ExportReceipt("PX0003", Date(10, 10, 2026), "C001", "W003");
    late->addLine(ReceiptLine("P002", 6, 4500));      // chỉ còn 5 cái còn hạn
    assert(!inv.processReceipt(late).ok);
    ExportReceipt* ok = new ExportReceipt("PX0004", Date(10, 10, 2026), "C001", "W003");
    ok->addLine(ReceiptLine("P002", 5, 4500));
    assert(inv.processReceipt(ok).ok);
    assert(cat.findWarehouse("W003")->getQty("P002") == 5);                 // còn 5 cái hết hạn

    // ---------- Báo cáo doanh thu ----------
    assert(report.revenueBetween(Date(1, 9, 2026), Date(31, 10, 2026)) == 60 * 33000 + 5 * 4500);
    assert(report.revenueBetween(Date(1, 10, 2026), Date(31, 10, 2026)) == 5 * 4500);
    assert(report.revenueBetween(Date(31, 10, 2026), Date(1, 9, 2026)) == 0);

    // ---------- Xoá có ràng buộc ----------
    assert(!cat.removeProduct("P001").ok);           // còn tồn
    assert(!cat.removeWarehouse("W001").ok);         // còn hàng
    assert(cat.addProduct(new Product("P003", "Tam", "cai", "X", 1, 0)).ok);
    assert(cat.removeProduct("P003").ok && cat.findProduct("P003") == nullptr);
    assert(cat.addWarehouse(new Warehouse("W009", "Trong", "")).ok);
    assert(cat.removeWarehouse("W009").ok);
    assert(cat.removeCustomer("C001").ok && !cat.removeCustomer("C001").ok);

    // ---------- Mã phiếu tiếp theo, ghi file ----------
    assert(inv.nextReceiptId("PX") == "PX0006" || inv.findReceipt(inv.nextReceiptId("PX")) == nullptr);
    std::string f = inv.findReceipt("PN0001")->toFileString();
    assert(f.find("IMPORT|PN0001|10/09/2026|S001|W001\nL|P001|B1|50|25000|01/09/2026|30/09/2026") == 0);
    assert(inv.findReceipt("DC0001")->getType() == "TRANSFER");

    std::cout << "test_Inventory: OK\n";
    return 0;
}
