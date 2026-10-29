#include "services/SearchService.h"

// SearchService chỉ GIỮ con trỏ Product* (không sở hữu, Catalog mới delete).
// Khi đổi tên/giá 1 mặt hàng: removeProduct(p, qty) TRƯỚC khi đổi, rồi indexProduct(p, qty) sau.

SearchService::SearchService() : nameTrie(), priceTree(), stockTree() {}

void SearchService::indexProduct(Product* p, int totalQty) {
    if (p == nullptr) return;
    nameTrie.insert(p->getName(), p->getId());
    priceTree.insert(p->getSalePrice(), p);
    stockTree.insert(totalQty, p);
}

void SearchService::removeProduct(Product* p, int totalQty) {
    if (p == nullptr) return;
    nameTrie.remove(p->getName());
    priceTree.remove(p->getSalePrice(), p);
    stockTree.remove(totalQty, p);
}

void SearchService::updateStock(Product* p, int oldQty, int newQty) {
    if (p == nullptr || oldQty == newQty) return;
    if (stockTree.remove(oldQty, p)) stockTree.insert(newQty, p);
}

// Trả về mã mặt hàng có tên bắt đầu bằng prefix (không phân biệt hoa/thường)
int SearchService::suggestByName(const std::string& prefix, std::string results[], int maxResults) const {
    return nameTrie.suggest(prefix, results, maxResults);
}

int SearchService::filterByPrice(double low, double high, Product* results[], int maxResults) const {
    return priceTree.rangeQuery(low, high, results, maxResults);
}

int SearchService::filterByStock(int low, int high, Product* results[], int maxResults) const {
    return stockTree.rangeQuery(low, high, results, maxResults);
}
