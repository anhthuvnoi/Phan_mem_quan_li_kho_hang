#include "ds/HashTable.h"

// Bảng băm xâu (separate chaining): mỗi bucket là danh sách liên kết đơn các Entry.
// Load factor = count / capacity; vượt 0.75 thì rehash sang số nguyên tố ~ gấp đôi.

namespace {

// Hàm băm đa thức cho chuỗi: h = s0*31^(n-1) + s1*31^(n-2) + ... (tràn unsigned là hợp lệ)
unsigned long hashKey(const std::string& s) {
    unsigned long h = 0;
    for (std::size_t i = 0; i < s.size(); i++) h = h * 31 + static_cast<unsigned char>(s[i]);
    return h;
}

unsigned long hashKey(int x) {
    return static_cast<unsigned long>(x);
}

bool isPrime(int x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (int d = 3; (long long)d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

int nextPrime(int x) {
    if (x <= 2) return 2;
    while (!isPrime(x)) x++;
    return x;
}

} // namespace

template <typename K, typename V>
HashTable<K, V>::HashTable(int cap) : buckets(nullptr), capacity(nextPrime(cap > 0 ? cap : 1)), count(0) {
    buckets = new Entry*[capacity];
    for (int i = 0; i < capacity; i++) buckets[i] = nullptr;
}

// Chỉ giải phóng Entry; nếu V là con trỏ thì lớp sở hữu (Catalog, Warehouse) tự delete
template <typename K, typename V>
HashTable<K, V>::~HashTable() {
    for (int i = 0; i < capacity; i++) {
        Entry* e = buckets[i];
        while (e != nullptr) {
            Entry* next = e->next;
            delete e;
            e = next;
        }
    }
    delete[] buckets;
}

template <typename K, typename V>
int HashTable<K, V>::hash(const K& key) const {
    return static_cast<int>(hashKey(key) % static_cast<unsigned long>(capacity));
}

template <typename K, typename V>
void HashTable<K, V>::rehash() {
    int oldCap = capacity;
    Entry** old = buckets;
    capacity = nextPrime(oldCap * 2 + 1);
    buckets = new Entry*[capacity];
    for (int i = 0; i < capacity; i++) buckets[i] = nullptr;
    for (int i = 0; i < oldCap; i++) {           // chuyển từng Entry sang bảng mới
        Entry* e = old[i];
        while (e != nullptr) {
            Entry* next = e->next;
            int idx = hash(e->key);
            e->next = buckets[idx];
            buckets[idx] = e;
            e = next;
        }
    }
    delete[] old;
}

// false nếu key đã có
template <typename K, typename V>
bool HashTable<K, V>::insert(const K& key, const V& value) {
    if (contains(key)) return false;
    if ((count + 1) * 4 > capacity * 3) rehash();      // load factor sẽ > 0.75
    int idx = hash(key);
    Entry* e = new Entry(key, value);
    e->next = buckets[idx];                           // chèn đầu chuỗi
    buckets[idx] = e;
    count++;
    return true;
}

// false nếu key chưa có
template <typename K, typename V>
bool HashTable<K, V>::update(const K& key, const V& value) {
    V* v = find(key);
    if (v == nullptr) return false;
    *v = value;
    return true;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K& key) {
    int idx = hash(key);
    Entry* prev = nullptr;
    for (Entry* e = buckets[idx]; e != nullptr; prev = e, e = e->next) {
        if (e->key == key) {
            if (prev == nullptr) buckets[idx] = e->next;
            else prev->next = e->next;
            delete e;
            count--;
            return true;
        }
    }
    return false;
}

// Trả con trỏ tới value (sửa được), nullptr nếu không có
template <typename K, typename V>
V* HashTable<K, V>::find(const K& key) {
    for (Entry* e = buckets[hash(key)]; e != nullptr; e = e->next)
        if (e->key == key) return &e->value;
    return nullptr;
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K& key) const {
    for (Entry* e = buckets[hash(key)]; e != nullptr; e = e->next)
        if (e->key == key) return true;
    return false;
}

template <typename K, typename V>
int HashTable<K, V>::size() const { return count; }

template <typename K, typename V>
void HashTable<K, V>::forEach(void (*visit)(const K& key, const V& value, void* ctx), void* ctx) const {
    if (visit == nullptr) return;
    for (int i = 0; i < capacity; i++)
        for (Entry* e = buckets[i]; e != nullptr; e = e->next) visit(e->key, e->value, ctx);
}

// ---------- Các kiểu được dùng trong dự án ----------
#include "models/Product.h"
#include "models/Supplier.h"
#include "models/Customer.h"
#include "models/Warehouse.h"
#include "models/StockItem.h"

template class HashTable<std::string, int>;
template class HashTable<std::string, std::string>;
template class HashTable<int, int>;
template class HashTable<std::string, Product*>;      // Catalog
template class HashTable<std::string, Supplier*>;
template class HashTable<std::string, Customer*>;
template class HashTable<std::string, Warehouse*>;
template class HashTable<std::string, StockItem*>;    // Warehouse: productId -> tồn kho
