#ifndef DS_BST_H
#define DS_BST_H

// Cây nhị phân tìm kiếm — dùng cho: lọc mặt hàng theo khoảng giá / khoảng tồn kho
// Gợi ý nâng cấp: AVL để tránh cây lệch
template <typename K, typename V>
class BST {
private:
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
        Node(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };
    Node* root;
    int count;

    void destroy(Node* node);

public:
    BST();
    ~BST();

    void insert(const K& key, const V& value);
    bool remove(const K& key, const V& value);
    // Lấy các value có key trong [low, high], trả về số phần tử
    int rangeQuery(const K& low, const K& high, V results[], int maxResults) const;
    int size() const;
};

#endif // DS_BST_H
