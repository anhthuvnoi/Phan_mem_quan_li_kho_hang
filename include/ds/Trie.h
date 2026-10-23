#ifndef DS_TRIE_H
#define DS_TRIE_H

// Cây tiền tố — dùng cho: gợi ý tên mặt hàng (autocomplete)
#include <string>
using namespace std;

class Trie {
private:
    static const int ALPHABET = 128;   // ASCII; tên có dấu -> chuẩn hoá bỏ dấu trước khi chèn
    struct Node {
        Node* children[ALPHABET];
        bool isEnd;
        string productId;
        Node();
    };
    Node* root;
    void destroy(Node* node);

    bool hasChildren(Node* node) const;
    bool removeHelper(Node* node, const string& name, size_t depth, bool& removed);
    void suggestHelper(Node* node, string result[], int maxResult, int& count) const;

public:
    Trie();
    ~Trie();

    void insert(const string& name, const string& productId);
    bool remove(const string& name);
    // Trả về tối đa maxResults productId có tên bắt đầu bằng prefix
    int suggest(const string& prefix, string results[], int maxResults) const;
};

#endif // DS_TRIE_H
