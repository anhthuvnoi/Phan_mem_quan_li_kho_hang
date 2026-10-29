#include "services/FileManager.h"
#include "services/ReportService.h"
#include "models/ImportReceipt.h"
#include "models/ExportReceipt.h"
#include "models/TransferReceipt.h"
#include <fstream>
#include <sstream>

// Các file trong dataDir (phân cách '|', dòng bắt đầu bằng '#' là chú thích):
//   products.txt, suppliers.txt, customers.txt, warehouses.txt, batches.txt (tồn kho theo lô)
//   receipts.txt : lịch sử phiếu (Receipt::toFileString). Khi nạp chỉ dựng lại lịch sử + doanh thu,
//                  KHÔNG chạy lại apply (tồn kho đã có sẵn trong batches.txt).

namespace {

bool skipLine(std::string& line) {
    if (!line.empty() && line.back() == '\r') line.pop_back();
    return line.empty() || line[0] == '#';
}

void splitFields(const std::string& line, std::string f[], int n) {
    std::istringstream ss(line);
    for (int i = 0; i < n; i++) {
        if (!std::getline(ss, f[i], '|')) f[i].clear();
    }
}

double toDouble(const std::string& s) {
    try { return std::stod(s); } catch (...) { return 0.0; }
}
int toInt(const std::string& s) {
    try { return std::stoi(s); } catch (...) { return 0; }
}

// Nạp 1 file danh mục: T là Product/Supplier/Customer/Warehouse; add là hàm thêm của Catalog
template <typename T>
int loadEntities(const std::string& path, Catalog& cat, Result (Catalog::*add)(T*), int& skipped) {
    std::ifstream in(path);
    if (!in) return -1;
    int n = 0;
    std::string line;
    while (std::getline(in, line)) {
        if (skipLine(line)) continue;
        T* obj = new T();
        obj->fromFileString(line);
        if ((cat.*add)(obj).ok) n++;
        else { delete obj; skipped++; }            // thiếu mã / trùng mã
    }
    return n;
}

// ---------- ghi file ----------
template <typename T>
void writeEntity(const std::string&, T* const& obj, void* ctx) {
    *static_cast<std::ofstream*>(ctx) << obj->toFileString() << "\n";
}

struct BatchWriter { std::ofstream* out; Warehouse* wh; int count; };
void writeBatch(Batch* const& b, void* ctx) {
    BatchWriter* w = static_cast<BatchWriter*>(ctx);
    *w->out << b->toFileString() << "\n";
    w->count++;
}
void writeProductBatches(const std::string& productId, Product* const&, void* ctx) {
    BatchWriter* w = static_cast<BatchWriter*>(ctx);
    StockItem* s = w->wh->getStock(productId);
    if (s != nullptr) s->getBatches().forEach(writeBatch, ctx);
}
struct AllBatches { const Catalog* cat; BatchWriter writer; };
void writeWarehouseBatches(const std::string&, Warehouse* const& wh, void* ctx) {
    AllBatches* a = static_cast<AllBatches*>(ctx);
    a->writer.wh = wh;
    a->cat->getProducts().forEach(writeProductBatches, &a->writer);
}
void writeReceipt(Receipt* const& r, void* ctx) {
    *static_cast<std::ofstream*>(ctx) << r->toFileString() << "\n";
}

} // namespace

FileManager::FileManager(const std::string& dataDir) : dataDir(dataDir) {}

Result FileManager::loadAll(Catalog& catalog, InventoryService& inv) {
    int skipped = 0;
    int nP = loadEntities<Product>(dataDir + "products.txt", catalog, &Catalog::addProduct, skipped);
    int nS = loadEntities<Supplier>(dataDir + "suppliers.txt", catalog, &Catalog::addSupplier, skipped);
    int nC = loadEntities<Customer>(dataDir + "customers.txt", catalog, &Catalog::addCustomer, skipped);
    int nW = loadEntities<Warehouse>(dataDir + "warehouses.txt", catalog, &Catalog::addWarehouse, skipped);
    if (nP < 0 || nW < 0)
        return Result(false, "Khong mo duoc products.txt / warehouses.txt trong " + dataDir);

    // Lô hàng -> gắn vào kho; bỏ qua lô trỏ tới kho / mặt hàng không tồn tại
    int nB = 0;
    std::ifstream bin(dataDir + "batches.txt");
    std::string line;
    while (bin && std::getline(bin, line)) {
        if (skipLine(line)) continue;
        Batch* b = new Batch();
        b->fromFileString(line);
        Warehouse* wh = catalog.findWarehouse(b->getWarehouseId());
        if (wh == nullptr || catalog.findProduct(b->getProductId()) == nullptr || b->getBatchId().empty()) {
            delete b;
            skipped++;
            continue;
        }
        wh->getOrCreateStock(b->getProductId())->addBatch(b);
        nB++;
    }

    // Lịch sử phiếu (không bắt buộc có file)
    int nR = 0;
    std::ifstream rin(dataDir + "receipts.txt");
    Receipt* cur = nullptr;
    while (rin && std::getline(rin, line)) {
        if (skipLine(line)) continue;
        std::string f[7];
        splitFields(line, f, 7);
        if (f[0] == "L") {                                   // dòng hàng của phiếu đang đọc
            if (cur == nullptr) continue;
            ReceiptLine l(f[1], toInt(f[3]), toDouble(f[4]));
            l.setBatchInfo(f[2], Date::fromString(f[5]), Date::fromString(f[6]));
            cur->addLine(l);
            continue;
        }
        Date d = Date::fromString(f[2]);
        if (f[0] == "IMPORT") cur = new ImportReceipt(f[1], d, f[3], f[4]);
        else if (f[0] == "EXPORT") cur = new ExportReceipt(f[1], d, f[3], f[4]);
        else if (f[0] == "TRANSFER") cur = new TransferReceipt(f[1], d, f[3], f[4]);
        else { cur = nullptr; skipped++; continue; }
        inv.getHistory().pushBack(cur);
        nR++;
    }
    // Dựng lại doanh thu từ các phiếu xuất
    for (int i = 0; i < inv.getHistory().size(); i++) {
        Receipt* r = inv.getHistory().at(i);
        if (r->getType() == "EXPORT") inv.getReport().addRevenue(r->getDate(), r->total());
    }

    std::string msg = "Da nap " + std::to_string(nP) + " mat hang, " + std::to_string(nS < 0 ? 0 : nS) +
                      " NCC, " + std::to_string(nC < 0 ? 0 : nC) + " khach hang, " + std::to_string(nW) +
                      " kho, " + std::to_string(nB) + " lo, " + std::to_string(nR) + " phieu";
    if (skipped > 0) msg += " (bo qua " + std::to_string(skipped) + " dong loi)";
    return Result(true, msg);
}

Result FileManager::saveAll(Catalog& catalog, InventoryService& inv) {
    std::ofstream p(dataDir + "products.txt"), s(dataDir + "suppliers.txt"), c(dataDir + "customers.txt"),
                  w(dataDir + "warehouses.txt"), b(dataDir + "batches.txt"), r(dataDir + "receipts.txt");
    if (!p || !s || !c || !w || !b || !r) return Result(false, "Khong ghi duoc file trong " + dataDir);

    p << "# id|name|unit|category|salePrice|minStock\n";
    catalog.getProducts().forEach(writeEntity<Product>, &p);
    s << "# id|name|phone|address|email|taxCode\n";
    catalog.getSuppliers().forEach(writeEntity<Supplier>, &s);
    c << "# id|name|phone|address|email|vip(0/1)\n";
    catalog.getCustomers().forEach(writeEntity<Customer>, &c);
    w << "# id|name|address\n";
    catalog.getWarehouses().forEach(writeEntity<Warehouse>, &w);

    b << "# batchId|productId|warehouseId|mfgDate|expDate|costPrice|qtyIn|qtyRemain\n";
    AllBatches all{&catalog, BatchWriter{&b, nullptr, 0}};
    catalog.getWarehouses().forEach(writeWarehouseBatches, &all);

    r << "# TYPE|id|date|doiTac/khoXuat|kho/khoNhap|soDong  +  L|productId|batchId|qty|unitPrice|mfg|exp\n";
    inv.getHistory().forEach(writeReceipt, &r);

    return Result(true, "Da luu " + std::to_string(catalog.getProducts().size()) + " mat hang, " +
                        std::to_string(all.writer.count) + " lo, " +
                        std::to_string(inv.getHistory().size()) + " phieu vao " + dataDir);
}
