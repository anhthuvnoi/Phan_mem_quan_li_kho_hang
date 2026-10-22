#ifndef SERVICES_SEARCHSERVICE_H
#define SERVICES_SEARCHSERVICE_H

#include "ds/Trie.h"
#include "ds/BST.h"
#include "models/Product.h"

// Tra cứu: gợi ý tên (Trie), lọc theo giá / tồn (BST)
class SearchService {
private:
    Trie nameTrie;
    BST<double, Product*> priceTree;
    BST<int, Product*> stockTree;

public:
    SearchService();

    void indexProduct(Product* p, int totalQty);
    void removeProduct(Product* p, int totalQty);
    void updateStock(Product* p, int oldQty, int newQty);

    int suggestByName(const std::string& prefix, std::string results[], int maxResults) const;
    int filterByPrice(double low, double high, Product* results[], int maxResults) const;
    int filterByStock(int low, int high, Product* results[], int maxResults) const;
};

#endif // SERVICES_SEARCHSERVICE_H
