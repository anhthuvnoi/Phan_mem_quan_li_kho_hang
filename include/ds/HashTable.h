#ifndef DS_HASHTABLE_H
#define DS_HASHTABLE_H

// Bảng băm (separate chaining) — dùng cho: danh mục Mặt hàng, NCC, Khách hàng, Kho
#include <string>

template <typename K, typename V>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    Entry** buckets;
    int capacity;
    int count;

    int hash(const K& key) const;   // TODO: cài hàm băm cho std::string (vd: djb2 / polynomial)
    void rehash();                   // khi load factor > 0.75

public:
    HashTable(int cap = 101);
    ~HashTable();

    bool insert(const K& key, const V& value);
    bool update(const K& key, const V& value);
    bool remove(const K& key);
    V* find(const K& key);
    bool contains(const K& key) const;
    int size() const;
};

#endif // DS_HASHTABLE_H
