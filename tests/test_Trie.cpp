#include <cassert>
#include <iostream>
#include <string>
#include "ds/Trie.h"

int main() {
    Trie t;
    std::string r[10];

    // 1. Rỗng
    assert(t.suggest("a", r, 10) == 0);
    assert(!t.remove("abc"));

    // 2. Chèn & gợi ý theo thứ tự từ điển
    t.insert("sua tuoi", "SP01");
    t.insert("sua chua", "SP02");
    t.insert("sua", "SP03");
    t.insert("banh mi", "SP04");
    t.insert("bia", "SP05");

    int n = t.suggest("sua", r, 10);
    assert(n == 3);
    assert(r[0] == "SP03" && r[1] == "SP02" && r[2] == "SP01");   // sua < sua chua < sua tuoi
    assert(t.suggest("b", r, 10) == 2 && r[0] == "SP04" && r[1] == "SP05");
    assert(t.suggest("x", r, 10) == 0);
    assert(t.suggest("", r, 10) == 5);                              // prefix rỗng -> tất cả

    // 3. Giới hạn maxResults
    assert(t.suggest("sua", r, 2) == 2 && r[0] == "SP03" && r[1] == "SP02");
    assert(t.suggest("sua", r, 0) == 0);

    // 4. Không phân biệt hoa/thường
    assert(t.suggest("SUA T", r, 10) == 1 && r[0] == "SP01");
    t.insert("Coca Cola", "SP06");
    assert(t.suggest("coca", r, 10) == 1 && r[0] == "SP06");

    // 5. Ghi đè productId khi chèn lại cùng tên
    t.insert("bia", "SP05B");
    assert(t.suggest("bia", r, 10) == 1 && r[0] == "SP05B");

    // 6. Xoá
    assert(t.remove("sua"));
    assert(!t.remove("sua"));              // đã xoá
    assert(!t.remove("su"));               // chỉ là tiền tố, không phải tên
    n = t.suggest("sua", r, 10);
    assert(n == 2 && r[0] == "SP02" && r[1] == "SP01");   // các tên dài hơn vẫn còn
    assert(t.remove("sua tuoi"));
    assert(t.remove("sua chua"));
    assert(t.suggest("s", r, 10) == 0);    // nhánh "s" đã được tỉa
    assert(t.suggest("b", r, 10) == 2);    // nhánh khác không ảnh hưởng

    // 7. Ký tự ngoài ASCII bị bỏ qua (không crash)
    t.insert("S\xe1\xbb\xaf" "a", "SP07");      // "Sữa" UTF-8 -> lưu như "sa"
    assert(t.suggest("sa", r, 10) == 1 && r[0] == "SP07");
    t.insert("", "SP08");                     // tên rỗng -> bỏ qua
    assert(t.suggest("", r, 10) == 4);

    // 8. Nhiều phần tử
    Trie big;
    for (int i = 0; i < 2000; i++) big.insert("item" + std::to_string(i), "P" + std::to_string(i));
    std::string many[2000];
    assert(big.suggest("item", many, 2000) == 2000);
    assert(big.suggest("item19", many, 2000) == 111);  // item19, item190..199, item1900..1999
    assert(big.remove("item19"));
    assert(big.suggest("item19", many, 2000) == 110);

    std::cout << "test_Trie: OK\n";
    return 0;
}
