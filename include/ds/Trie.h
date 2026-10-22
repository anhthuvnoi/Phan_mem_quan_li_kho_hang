#ifndef DS_TRIE_H
#define DS_TRIE_H

// Cây tiền tố — dùng cho: gợi ý tên mặt hàng (autocomplete)
#include <string>

class Trie {
private:
    static const int ALPHABET = 128;   // ASCII; tên có dấu -> chuẩn hoá bỏ dấu trước khi chèn
    struct Node {
        Node* children[ALPHABET];
        bool isEnd;
        std::string productId;
        Node();
    };
    Node* root;
    void destroy(Node* node);

public:
    Trie();
    ~Trie();

    void insert(const std::string& name, const std::string& productId);
    bool remove(const std::string& name);
    // Trả về tối đa maxResults productId có tên bắt đầu bằng prefix
    int suggest(const std::string& prefix, std::string results[], int maxResults) const;
};

#endif // DS_TRIE_H
