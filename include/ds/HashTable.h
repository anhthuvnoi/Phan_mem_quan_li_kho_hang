#ifndef DS_HASHTABLE_H
#define DS_HASHTABLE_H

#include <string>

template <typename K>

// vì sao phải ép kiểu unsigned long??
// khóa key có thể là các kiểu số có dấu như int, long qua quá trình tính toán có thể ra số âm (nếu ko cẩn thận), hàm băm k bao giờ được phép âm, kiểm soát được tràn số
struct DefaultHash {
    unsigned long operator()(const K& key) const {
        return static_cast<unsigned long>(key);
    }
};

// std::string: h = s[0]*31^(n-1) + s[1]*31^(n-2) + ... (tràn số unsigned là hợp lệ)
// phiên bản chuyên hóa cho chuỗi, băm đa thức với hệ số nhân kinh điển
template <>
struct DefaultHash<std::string> {
    unsigned long operator()(const std::string& key) const {
        unsigned long h = 0;
        for (std::size_t i = 0; i < key.size(); i++) {
            h = h * 31 + static_cast<unsigned char>(key[i]);
        }
        return h;
    }
};

template <typename K, typename V, typename Hash = DefaultHash<K>>
class HashTable {
private:
    // cấu trúc lưu trữ: mảng con trỏ kết hợp danh sách liên kết
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    Entry** buckets; 
    int capacity;
    int count;
    Hash hasher;

    int hash(const K& key) const;      // chỉ số bucket trong [0, capacity)
    void rehash();                     // khi load factor > 0.75
    Entry* findEntry(const K& key) const;
    void freeAll();
    void copyFrom(const HashTable& other);

    static bool isPrime(int x);
    static int nextPrime(int x);

public:
    HashTable(int cap = 101);
    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);
    ~HashTable();

    bool insert(const K& key, const V& value);   
    bool update(const K& key, const V& value);   
    bool remove(const K& key);                  
    V* find(const K& key);                       
    const V* find(const K& key) const;
    bool contains(const K& key) const;
    int size() const;
    bool empty() const;
    void clear();                             

    // Duyệt toàn bộ: f(const K& key, V& value)
    template <typename F> void forEach(F f);
    template <typename F> void forEach(F f) const;   // f(const K&, const V&)

    // Chép tối đa maxResults key ra mảng, trả về số key đã chép
    int getKeys(K results[], int maxResults) const;
};

#include "ds/HashTable.cpp"

#endif // DS_HASHTABLE_H
