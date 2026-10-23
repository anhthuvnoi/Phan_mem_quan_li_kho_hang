#include "ds/BST.h"
using namespace std;
template <typename K, typename V>
BST<K, V>::BST() : root(nullptr), count(0) {}
template <typename K, typename V>
BST<K, V>::~BST() {
    destroy(root);
    root = nullptr;
    count = 0;
}
template <typename K, typename V>
void BST<K, V>::destroy(Node* node) {
    if (node != nullptr) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}
template <typename K, typename V>
typename BST<K, V>::Node* BST<K, V>::findMin(Node* node) const {
    while(node != nullptr && node->left != nullptr){
        node = node->left;
    }
    return node;
}
template <typename K, typename V>
typename BST<K, V>::Node* BST<K, V>::insertHelper(Node* node, const K& key, const V& value) {
    if(node == nullptr){
        return new Node(key, value);
    }
    if(key < node->key){
        node->left = insertHelper(node->left, key, value);
    }
    if(key > node->key){
        node->right = insertHelper(node->right, key, value);
    }
    return node;
}
template <typename K, typename V>
void BST<K, V>::insert(const K& key, const V& value){
    root = insertHelper(root, key, value);
    count++;
}
template <typename K, typename V>
typename BST<K, V>::Node* BST<K, V>::removeHelper(Node* node, const K& key, const V& value, bool& removed) {
    if (node == nullptr) return nullptr;

    if (key < node->key) {
        node->left = removeHelper(node->left, key, value, removed);
    } else if (key > node->key) {
        node->right = removeHelper(node->right, key, value, removed);
    } else {
        // Khóa trùng khớp
        if (node->value == value) {
            removed = true;
            // Trường hợp 0 hoặc 1 con
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // Trường hợp 2 con
            Node* successor = findMin(node->right);
            node->key = successor->key;
            node->value = successor->value;
            node->right = removeHelper(node->right, successor->key, successor->value, removed);
        } else {
            // Trùng key nhưng khác value -> tiếp tục tìm bên phải
            node->right = removeHelper(node->right, key, value, removed);
        }
    }
    return node;
}

template <typename K, typename V>
bool BST<K, V>::remove(const K& key, const V& value) {
    bool removed = false;
    root = removeHelper(root, key, value, removed);
    if (removed) {
        count--;
    }
    return removed;
}

// 5. Range Query & RangeQueryHelper (Duyệt In-order)
template <typename K, typename V>
void BST<K, V>::rangeQueryHelper(Node* node, const K& low, const K& high, 
                                 V results[], int maxResults, int& currentCount) const {
    if (node == nullptr || currentCount >= maxResults) return;

    if (node->key > low) {
        rangeQueryHelper(node->left, low, high, results, maxResults, currentCount);
    }

    if (node->key >= low && node->key <= high) {
        if (currentCount < maxResults) {
            results[currentCount++] = node->value;
        }
    }

    if (node->key < high) {
        rangeQueryHelper(node->right, low, high, results, maxResults, currentCount);
    }
}

template <typename K, typename V>
int BST<K, V>::rangeQuery(const K& low, const K& high, V results[], int maxResults) const {
    int currentCount = 0;
    rangeQueryHelper(root, low, high, results, maxResults, currentCount);
    return currentCount;
}

// 6. Size
template <typename K, typename V>
int BST<K, V>::size() const {
    return count;
}