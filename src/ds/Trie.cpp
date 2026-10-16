#include "ds/Trie.h"
Trie::Node::Node() : isEnd(false), productId("") {
    for(int i=0; i<ALPHABET; i++){
        children[i] = nullptr;
    }
}
Trie::Trie() {
    root = new Node();
}
Trie::~Trie() {
    destroy(root);
    root = nullptr;
}
void Trie::destroy(Node* node){
    if(node == nullptr) return;
    for(int i=0; i<ALPHABET; i++){
        if(node->children[i] != nullptr){
            destroy(node->children[i]);
        }
    }
    delete node;
}
void Trie::insert(const string& name, const string& productId) {
    if(name.empty()) return;
    Node* current = root;
    for(char ch : name){
        unsigned char index = static_cast<unsigned char> (ch);
        if(index >= ALPHABET) continue;
        if(current->children[index] == nullptr){
            current->children[index] = new Node();
        }
        current = current->children[index];
    }  
    current->isEnd = true;
    current->productId = productId; 
}
bool Trie::hasChildren(Node* node) const {
    if(node == nullptr) return false;
    for(int i=0; i<ALPHABET; i++){
        if(node->children[i] != nullptr) return true;
    }
    return false;
}
bool Trie::removeHelper(Node* node, const string& name, size_t depth, bool& removed) {
    if (node == nullptr) return false;

    if (depth == name.size()) {
        if (node->isEnd) {
            node->isEnd = false;
            node->productId = "";
            removed = true;
            return !hasChildren(node);
        }
        return false;
    }

    unsigned char index = static_cast<unsigned char>(name[depth]);
    if (index >= ALPHABET || node->children[index] == nullptr) {
        return false;
    }

    bool shouldDeleteChild = removeHelper(node->children[index], name, depth + 1, removed);

    if (shouldDeleteChild) {
        delete node->children[index];
        node->children[index] = nullptr;
        return !node->isEnd && !hasChildren(node);
    }

    return false;
}

bool Trie::remove(const std::string& name) {
    if (name.empty() || root == nullptr) return false;
    bool removed = false;
    removeHelper(root, name, 0, removed);
    return removed;
}

void Trie::suggestHelper(Node* node, string results[], int maxResults, int& count) const {
    if (node == nullptr || count >= maxResults) return;

    if (node->isEnd) {
        results[count++] = node->productId;
        if (count >= maxResults) return;
    }

    for (int i = 0; i < ALPHABET; ++i) {
        if (node->children[i] != nullptr) {
            suggestHelper(node->children[i], results, maxResults, count);
            if (count >= maxResults) return;
        }
    }
}

int Trie::suggest(const std::string& prefix, std::string results[], int maxResults) const {
    if (root == nullptr || maxResults <= 0) return 0;

    Node* current = root;
    for (char ch : prefix) {
        unsigned char index = static_cast<unsigned char>(ch);
        if (index >= ALPHABET || current->children[index] == nullptr) {
            return 0; 
        }
        current = current->children[index];
    }

    int count = 0;
    suggestHelper(current, results, maxResults, count);
    return count;
}
