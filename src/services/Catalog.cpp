#include "services/Catalog.h"

// Catalog SỞ HỮU mọi Product/Supplier/Customer/Warehouse:
//  - add*   : thành công -> Catalog giữ con trỏ; thất bại (null, mã rỗng, trùng mã) -> người gọi tự delete
//  - remove*: delete đối tượng; từ chối nếu còn tồn kho liên quan
// Thông báo không dấu để hiển thị đúng trên console Windows.

namespace {
template <typename T>
void deleteValue(const std::string&, T* const& p, void*) { delete p; }

struct QtyOf { std::string productId; int total; };
void addQtyOf(const std::string&, Warehouse* const& w, void* ctx) {
    QtyOf* q = static_cast<QtyOf*>(ctx);
    q->total += w->getQty(q->productId);
}

struct WhTotal { Warehouse* wh; int total; };
void addWhQty(const std::string& productId, Product* const&, void* ctx) {
    WhTotal* t = static_cast<WhTotal*>(ctx);
    t->total += t->wh->getQty(productId);
}
} // namespace

Catalog::Catalog() : products(), suppliers(), customers(), warehouses() {}

Catalog::~Catalog() {
    products.forEach(deleteValue<Product>, nullptr);
    suppliers.forEach(deleteValue<Supplier>, nullptr);
    customers.forEach(deleteValue<Customer>, nullptr);
    warehouses.forEach(deleteValue<Warehouse>, nullptr);
}

// ---------- Mặt hàng ----------
Result Catalog::addProduct(Product* p) {
    if (p == nullptr || p->getId().empty()) return Result(false, "Mat hang khong hop le (thieu ma)");
    if (!products.insert(p->getId(), p)) return Result(false, "Ma mat hang " + p->getId() + " da ton tai");
    return Result(true, "Da them mat hang " + p->getId());
}

Result Catalog::updateProduct(const Product& p) {
    Product* cur = findProduct(p.getId());
    if (cur == nullptr) return Result(false, "Khong tim thay mat hang " + p.getId());
    *cur = p;
    return Result(true, "Da cap nhat mat hang " + p.getId());
}

Result Catalog::removeProduct(const std::string& id) {
    Product* cur = findProduct(id);
    if (cur == nullptr) return Result(false, "Khong tim thay mat hang " + id);
    QtyOf q{id, 0};
    warehouses.forEach(addQtyOf, &q);
    if (q.total > 0) return Result(false, "Mat hang " + id + " con ton " + std::to_string(q.total) + ", khong the xoa");
    products.remove(id);
    delete cur;
    return Result(true, "Da xoa mat hang " + id);
}

Product* Catalog::findProduct(const std::string& id) {
    Product** p = products.find(id);
    return p ? *p : nullptr;
}

// ---------- Nhà cung cấp ----------
Result Catalog::addSupplier(Supplier* s) {
    if (s == nullptr || s->getId().empty()) return Result(false, "Nha cung cap khong hop le (thieu ma)");
    if (!suppliers.insert(s->getId(), s)) return Result(false, "Ma NCC " + s->getId() + " da ton tai");
    return Result(true, "Da them nha cung cap " + s->getId());
}

Result Catalog::updateSupplier(const Supplier& s) {
    Supplier* cur = findSupplier(s.getId());
    if (cur == nullptr) return Result(false, "Khong tim thay nha cung cap " + s.getId());
    *cur = s;
    return Result(true, "Da cap nhat nha cung cap " + s.getId());
}

Result Catalog::removeSupplier(const std::string& id) {
    Supplier* cur = findSupplier(id);
    if (cur == nullptr) return Result(false, "Khong tim thay nha cung cap " + id);
    suppliers.remove(id);
    delete cur;
    return Result(true, "Da xoa nha cung cap " + id);
}

Supplier* Catalog::findSupplier(const std::string& id) {
    Supplier** s = suppliers.find(id);
    return s ? *s : nullptr;
}

// ---------- Khách hàng ----------
Result Catalog::addCustomer(Customer* c) {
    if (c == nullptr || c->getId().empty()) return Result(false, "Khach hang khong hop le (thieu ma)");
    if (!customers.insert(c->getId(), c)) return Result(false, "Ma khach hang " + c->getId() + " da ton tai");
    return Result(true, "Da them khach hang " + c->getId());
}

Result Catalog::updateCustomer(const Customer& c) {
    Customer* cur = findCustomer(c.getId());
    if (cur == nullptr) return Result(false, "Khong tim thay khach hang " + c.getId());
    *cur = c;
    return Result(true, "Da cap nhat khach hang " + c.getId());
}

Result Catalog::removeCustomer(const std::string& id) {
    Customer* cur = findCustomer(id);
    if (cur == nullptr) return Result(false, "Khong tim thay khach hang " + id);
    customers.remove(id);
    delete cur;
    return Result(true, "Da xoa khach hang " + id);
}

Customer* Catalog::findCustomer(const std::string& id) {
    Customer** c = customers.find(id);
    return c ? *c : nullptr;
}

// ---------- Kho ----------
Result Catalog::addWarehouse(Warehouse* w) {
    if (w == nullptr || w->getId().empty()) return Result(false, "Kho khong hop le (thieu ma)");
    if (!warehouses.insert(w->getId(), w)) return Result(false, "Ma kho " + w->getId() + " da ton tai");
    return Result(true, "Da them kho " + w->getId());
}

// Không chép cả đối tượng (Warehouse đang giữ tồn kho) -> chỉ cập nhật tên, địa chỉ
Result Catalog::updateWarehouse(const Warehouse& w) {
    Warehouse* cur = findWarehouse(w.getId());
    if (cur == nullptr) return Result(false, "Khong tim thay kho " + w.getId());
    cur->setName(w.getName());
    cur->setAddress(w.getAddress());
    return Result(true, "Da cap nhat kho " + w.getId());
}

Result Catalog::removeWarehouse(const std::string& id) {
    Warehouse* cur = findWarehouse(id);
    if (cur == nullptr) return Result(false, "Khong tim thay kho " + id);
    WhTotal t{cur, 0};
    products.forEach(addWhQty, &t);
    if (t.total > 0) return Result(false, "Kho " + id + " con " + std::to_string(t.total) + " hang, khong the xoa");
    warehouses.remove(id);
    delete cur;
    return Result(true, "Da xoa kho " + id);
}

Warehouse* Catalog::findWarehouse(const std::string& id) {
    Warehouse** w = warehouses.find(id);
    return w ? *w : nullptr;
}

const HashTable<std::string, Product*>& Catalog::getProducts() const { return products; }
const HashTable<std::string, Supplier*>& Catalog::getSuppliers() const { return suppliers; }
const HashTable<std::string, Customer*>& Catalog::getCustomers() const { return customers; }
const HashTable<std::string, Warehouse*>& Catalog::getWarehouses() const { return warehouses; }
