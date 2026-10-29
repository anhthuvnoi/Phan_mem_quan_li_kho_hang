#include <iostream>
#include <string>
#include "services/WarehouseSystem.h"
#include "models/ImportReceipt.h"
#include "models/ExportReceipt.h"
#include "models/TransferReceipt.h"

// Giao diện dòng lệnh (CLI). Chỉ làm việc qua WarehouseSystem.
// Giao diện đồ hoạ (nếu có) đặt trong thư mục ui/ và cũng chỉ gọi WarehouseSystem.

namespace {

std::string ask(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    if (!std::getline(std::cin, s)) return "";
    if (!s.empty() && s.back() == '\r') s.pop_back();
    return s;
}

int askInt(const std::string& prompt) {
    try { return std::stoi(ask(prompt)); } catch (...) { return -1; }
}

double askDouble(const std::string& prompt) {
    try { return std::stod(ask(prompt)); } catch (...) { return -1; }
}

Date askDate(const std::string& prompt) {
    while (true) {
        Date d = Date::fromString(ask(prompt + " (dd/mm/yyyy): "));
        if (d.isValid()) return d;
        std::cout << "  Ngay khong hop le, nhap lai.\n";
    }
}

void show(const Result& r) {
    std::cout << (r.ok ? "  [OK] " : "  [LOI] ") << r.message << "\n";
}

// ---------- Tổng tồn 1 mặt hàng ở mọi kho ----------
struct QtyCtx { std::string productId; int total; };
void sumQty(const std::string&, Warehouse* const& w, void* ctx) {
    QtyCtx* q = static_cast<QtyCtx*>(ctx);
    q->total += w->getQty(q->productId);
}
int totalQty(WarehouseSystem& sys, const std::string& productId) {
    QtyCtx q{productId, 0};
    sys.getCatalog().getWarehouses().forEach(sumQty, &q);
    return q.total;
}

// Ghi nhớ tồn trước khi xử lý phiếu để cập nhật cây tìm theo tồn (SearchService) sau đó
const int MAX_LINES = 50;
struct Snapshot { std::string ids[MAX_LINES]; int before[MAX_LINES]; int n = 0; };

void remember(WarehouseSystem& sys, Snapshot& snap, const std::string& productId) {
    for (int i = 0; i < snap.n; i++) if (snap.ids[i] == productId) return;
    if (snap.n == MAX_LINES) return;
    snap.ids[snap.n] = productId;
    snap.before[snap.n] = totalQty(sys, productId);
    snap.n++;
}

void syncSearch(WarehouseSystem& sys, const Snapshot& snap) {
    for (int i = 0; i < snap.n; i++) {
        Product* p = sys.getCatalog().findProduct(snap.ids[i]);
        sys.getSearch().updateStock(p, snap.before[i], totalQty(sys, snap.ids[i]));
    }
}

// ---------- In danh sách ----------
void printProduct(const std::string& id, Product* const& p, void* ctx) {
    WarehouseSystem* sys = static_cast<WarehouseSystem*>(ctx);
    std::cout << "  " << p->toString() << " | Ton: " << totalQty(*sys, id) << "\n";
}
void printEntity(const std::string&, Warehouse* const& w, void*) { std::cout << "  " << w->toString() << "\n"; }
void printSupplier(const std::string&, Supplier* const& s, void*) { std::cout << "  " << s->toString() << "\n"; }
void printCustomer(const std::string&, Customer* const& c, void*) { std::cout << "  " << c->toString() << "\n"; }
void printBatch(Batch* const& b, void*) { std::cout << "    " << b->toString() << "\n"; }
void printReceipt(Receipt* const& r, void*) { std::cout << "  " << r->toString() << "\n"; }

void listAll(WarehouseSystem& sys) {
    std::cout << "\n-- Mat hang --\n";
    sys.getCatalog().getProducts().forEach(printProduct, &sys);
    std::cout << "-- Kho --\n";
    sys.getCatalog().getWarehouses().forEach(printEntity, nullptr);
    std::cout << "-- Nha cung cap --\n";
    sys.getCatalog().getSuppliers().forEach(printSupplier, nullptr);
    std::cout << "-- Khach hang --\n";
    sys.getCatalog().getCustomers().forEach(printCustomer, nullptr);
}

void showStock(WarehouseSystem& sys) {
    Warehouse* w = sys.getCatalog().findWarehouse(ask("Ma kho: "));
    if (w == nullptr) { std::cout << "  Khong tim thay kho\n"; return; }
    std::string pid = ask("Ma mat hang: ");
    StockItem* s = w->getStock(pid);
    if (s == nullptr) { std::cout << "  Kho chua co mat hang nay\n"; return; }
    std::cout << "  Tong ton: " << s->getTotalQty() << "\n";
    s->getBatches().forEach(printBatch, nullptr);
}

// ---------- Phiếu ----------
void runReceipt(WarehouseSystem& sys, Receipt* r, Snapshot& snap) {
    Result res = sys.getInventory().processReceipt(r);
    show(res);
    if (res.ok) syncSearch(sys, snap);
    else delete r;                       // phiếu lỗi không được lưu -> tự giải phóng
}

void importGoods(WarehouseSystem& sys) {
    // Đọc từng giá trị vào biến riêng: thứ tự tính các đối số của 1 lời gọi hàm là KHÔNG xác định
    std::string id = ask("Ma phieu nhap: ");
    Date d = askDate("Ngay nhap");
    std::string supplierId = ask("Ma NCC: ");
    std::string warehouseId = ask("Ma kho: ");
    ImportReceipt* r = new ImportReceipt(id, d, supplierId, warehouseId);
    Snapshot snap;
    int n = askInt("So dong hang: ");
    for (int i = 0; i < n && i < MAX_LINES; i++) {
        std::cout << " Dong " << i + 1 << ":\n";
        std::string pid = ask("  Ma mat hang: ");
        int qty = askInt("  So luong: ");
        double price = askDouble("  Don gia nhap: ");
        ReceiptLine l(pid, qty, price);
        Date mfg = askDate("  NSX");
        Date exp = askDate("  HSD");
        std::string batchId = ask("  Ma lo (Enter = tu sinh): ");
        l.setBatchInfo(batchId, mfg, exp);
        r->addLine(l);
        remember(sys, snap, pid);
    }
    runReceipt(sys, r, snap);
}

void exportGoods(WarehouseSystem& sys) {
    std::string id = ask("Ma phieu xuat: ");
    Date d = askDate("Ngay xuat");
    std::string customerId = ask("Ma khach hang: ");
    std::string warehouseId = ask("Ma kho: ");
    ExportReceipt* r = new ExportReceipt(id, d, customerId, warehouseId);
    Snapshot snap;
    int n = askInt("So dong hang: ");
    for (int i = 0; i < n && i < MAX_LINES; i++) {
        std::cout << " Dong " << i + 1 << ":\n";
        std::string pid = ask("  Ma mat hang: ");
        Product* p = sys.getCatalog().findProduct(pid);
        int qty = askInt("  So luong: ");
        double price = p ? p->getSalePrice() : 0;
        std::cout << "  Don gia ban: " << price << "\n";
        r->addLine(ReceiptLine(pid, qty, price));
        remember(sys, snap, pid);
    }
    runReceipt(sys, r, snap);
}

void transferGoods(WarehouseSystem& sys) {
    std::string id = ask("Ma phieu dieu chuyen: ");
    Date d = askDate("Ngay");
    std::string pid = ask("Ma mat hang: ");
    std::string to = ask("Kho nhap: ");
    std::string suggest = sys.getInventory().suggestSourceWarehouse(pid, to);
    std::cout << "  Goi y kho xuat (ton nhieu nhat): " << (suggest.empty() ? "(khong co)" : suggest) << "\n";
    std::string from = ask("Kho xuat (Enter = theo goi y): ");
    if (from.empty()) from = suggest;
    TransferReceipt* r = new TransferReceipt(id, d, from, to);
    int qty = askInt("So luong: ");
    r->addLine(ReceiptLine(pid, qty, 0));
    Snapshot snap;
    remember(sys, snap, pid);
    runReceipt(sys, r, snap);
}

// ---------- Đơn hàng ----------
void createOrder(WarehouseSystem& sys) {
    std::string id = ask("Ma don: ");
    std::string cid = ask("Ma khach hang: ");
    std::string wid = ask("Ma kho: ");
    Date d = askDate("Ngay");
    Customer* c = sys.getCatalog().findCustomer(cid);
    bool urgent = (c != nullptr && c->isVIP()) || ask("Don khan? (y/n): ") == "y";
    Order* o = new Order(id, cid, wid, d, urgent);
    int n = askInt("So dong hang: ");
    for (int i = 0; i < n && i < MAX_LINES; i++) {
        std::string pid = ask("  Ma mat hang: ");
        Product* p = sys.getCatalog().findProduct(pid);
        int qty = askInt("  So luong: ");
        o->addLine(ReceiptLine(pid, qty, p ? p->getSalePrice() : 0));
    }
    sys.getOrders().enqueue(o);
    std::cout << "  Da xep don " << id << (urgent ? " len DAU" : " vao cuoi") << " hang doi. Dang cho: "
              << sys.getOrders().pendingCount() << "\n";
}

void collectOrderLine(const ReceiptLine& l, void* ctx) {
    void** pair = static_cast<void**>(ctx);
    remember(*static_cast<WarehouseSystem*>(pair[0]), *static_cast<Snapshot*>(pair[1]), l.getProductId());
}

void processOrder(WarehouseSystem& sys) {
    Order* o = sys.getOrders().peekNext();
    Snapshot snap;
    if (o != nullptr) {
        void* ctx[2] = {&sys, &snap};
        o->getLines().forEach(collectOrderLine, ctx);
    }
    Result r = sys.getOrders().processNext();
    show(r);
    if (r.ok) syncSearch(sys, snap);
}

// ---------- Tra cứu, cảnh báo, báo cáo ----------
void searchProducts(WarehouseSystem& sys) {
    std::string mode = ask("1. Goi y theo ten  2. Loc theo gia  3. Loc theo ton: ");
    const int MAX = 50;
    if (mode == "1") {
        std::string ids[MAX];
        int n = sys.getSearch().suggestByName(ask("Tien to ten: "), ids, MAX);
        for (int i = 0; i < n; i++) {
            Product* p = sys.getCatalog().findProduct(ids[i]);
            if (p) std::cout << "  " << p->toString() << "\n";
        }
        std::cout << "  (" << n << " ket qua)\n";
    } else {
        Product* res[MAX];
        int n;
        if (mode == "2") {
            double lo = askDouble("Gia tu: ");
            double hi = askDouble("Den: ");
            n = sys.getSearch().filterByPrice(lo, hi, res, MAX);
        } else {
            int lo = askInt("Ton tu: ");
            int hi = askInt("Den: ");
            n = sys.getSearch().filterByStock(lo, hi, res, MAX);
        }
        for (int i = 0; i < n; i++) std::cout << "  " << res[i]->toString() << "\n";
        std::cout << "  (" << n << " ket qua)\n";
    }
}

void alerts(WarehouseSystem& sys) {
    Date today = askDate("Hom nay");
    int days = askInt("Canh bao truoc bao nhieu ngay: ");
    const int MAX = 100;
    Batch* batches[MAX];
    int n = sys.getAlerts().nearExpiry(today, days, batches, MAX);
    std::cout << "-- Lo can date / het han (" << n << ") --\n";
    for (int i = 0; i < n; i++) {
        int left = batches[i]->daysToExpire(today);
        std::cout << "  " << batches[i]->toString()
                  << (left < 0 ? "  => DA HET HAN" : "  => con " + std::to_string(left) + " ngay") << "\n";
    }
    std::string ids[MAX];
    int m = sys.getAlerts().lowStock(ids, MAX);
    std::cout << "-- Ton duoi muc toi thieu (" << m << ") --\n";
    for (int i = 0; i < m; i++) {
        Product* p = sys.getCatalog().findProduct(ids[i]);
        std::cout << "  " << ids[i] << " - " << (p ? p->getName() : "") << " | Ton: " << totalQty(sys, ids[i])
                  << " / toi thieu " << (p ? p->getMinStock() : 0) << "\n";
    }
}

void revenue(WarehouseSystem& sys) {
    Date from = askDate("Tu ngay");
    Date to = askDate("Den ngay");
    std::cout << "  Doanh thu: " << static_cast<long long>(sys.getReport().revenueBetween(from, to) + 0.5) << "\n";
}

} // namespace

int main() {
    WarehouseSystem sys;
    std::cout << "=== Phan mem Quan ly Kho hang - PBL2 ===\n";
    show(sys.load());

    while (true) {
        std::cout << "\n 1. Xem danh muc          2. Xem ton theo lo\n"
                     " 3. Nhap kho              4. Xuat kho\n"
                     " 5. Dieu chuyen kho       6. Tao don hang\n"
                     " 7. Soan don tiep theo    8. Tra cuu mat hang\n"
                     " 9. Canh bao              10. Doanh thu\n"
                     " 11. Lich su phieu        12. Luu\n"
                     " 0. Luu va thoat\n";
        std::string c = ask("Chon: ");
        if (!std::cin) break;
        if (c == "1") listAll(sys);
        else if (c == "2") showStock(sys);
        else if (c == "3") importGoods(sys);
        else if (c == "4") exportGoods(sys);
        else if (c == "5") transferGoods(sys);
        else if (c == "6") createOrder(sys);
        else if (c == "7") processOrder(sys);
        else if (c == "8") searchProducts(sys);
        else if (c == "9") alerts(sys);
        else if (c == "10") revenue(sys);
        else if (c == "11") sys.getInventory().getHistory().forEach(printReceipt, nullptr);
        else if (c == "12") show(sys.save());
        else if (c == "0") break;
    }
    show(sys.save());
    return 0;
}
