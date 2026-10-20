#include <cassert>
#include <iostream>
#include <string>
#include "ds/HashTable.h"

int main() {
    // 1. Cơ bản với key string (như mã mặt hàng)
    HashTable<std::string, int> t;
    assert(t.empty() && t.size() == 0);
    assert(t.insert("SP001", 10));
    assert(t.insert("SP002", 20));
    assert(!t.insert("SP001", 99));            // trùng key
    assert(t.size() == 2);
    assert(t.find("SP001") && *t.find("SP001") == 10);
    assert(t.find("SP999") == nullptr);
    assert(t.contains("SP002") && !t.contains("SP003"));

    // 2. update
    assert(t.update("SP001", 11));
    assert(*t.find("SP001") == 11);
    assert(!t.update("SP404", 1));
    *t.find("SP002") += 5;                      // sửa qua con trỏ
    assert(*t.find("SP002") == 25);

    // 3. remove
    assert(t.remove("SP001"));
    assert(!t.remove("SP001"));
    assert(!t.contains("SP001") && t.size() == 1);

    // 4. Nhiều phần tử -> rehash nhiều lần, dữ liệu vẫn đúng
    HashTable<std::string, int> big(3);
    const int N = 20000;
    for (int i = 0; i < N; i++) assert(big.insert("KEY" + std::to_string(i), i));
    assert(big.size() == N);
    for (int i = 0; i < N; i++) {
        int* v = big.find("KEY" + std::to_string(i));
        assert(v && *v == i);
    }
    for (int i = 0; i < N; i += 2) assert(big.remove("KEY" + std::to_string(i)));
    assert(big.size() == N / 2);
    for (int i = 0; i < N; i++) assert(big.contains("KEY" + std::to_string(i)) == (i % 2 == 1));

    // 5. forEach / getKeys
    long long sum = 0;
    int cnt = 0;
    big.forEach([&](const std::string&, int& v) { sum += v; cnt++; });
    long long expected = 0;
    for (int i = 1; i < N; i += 2) expected += i;
    assert(cnt == N / 2 && sum == expected);

    const HashTable<std::string, int>& cref = big;
    int cnt2 = 0;
    cref.forEach([&](const std::string&, const int&) { cnt2++; });
    assert(cnt2 == N / 2);
    assert(cref.find("KEY1") && *cref.find("KEY1") == 1);

    std::string keys[5];
    assert(t.getKeys(keys, 5) == 1 && keys[0] == "SP002");

    // 6. Key số nguyên (kể cả âm)
    HashTable<int, std::string> it;
    assert(it.insert(-5, "am"));
    assert(it.insert(42, "x"));
    assert(*it.find(-5) == "am" && *it.find(42) == "x");

    // 7. Value là con trỏ — bảng không delete, người dùng tự giải phóng
    HashTable<std::string, int*> ptrs;
    for (int i = 0; i < 10; i++) ptrs.insert("P" + std::to_string(i), new int(i));
    ptrs.forEach([](const std::string&, int*& p) { delete p; p = nullptr; });
    assert(*ptrs.find("P3") == nullptr);

    // 8. Copy độc lập, clear
    HashTable<std::string, int> c1;
    c1.insert("A", 1); c1.insert("B", 2);
    HashTable<std::string, int> c2(c1);
    c2.update("A", 100); c2.insert("C", 3);
    assert(*c1.find("A") == 1 && !c1.contains("C") && c2.size() == 3);
    HashTable<std::string, int> c3;
    c3 = c2; c3 = c3;
    assert(c3.size() == 3 && *c3.find("A") == 100);
    c3.clear();
    assert(c3.empty() && !c3.contains("A"));
    assert(c3.insert("A", 5) && *c3.find("A") == 5);

    std::cout << "test_HashTable: OK\n";
    return 0;
}
