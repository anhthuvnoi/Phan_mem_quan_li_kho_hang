#ifndef SERVICES_ALERTSERVICE_H
#define SERVICES_ALERTSERVICE_H

#include "ds/MinHeap.h"
#include "models/Batch.h"
#include "models/Date.h"
#include "services/Catalog.h"

// Cảnh báo hàng cận date + đề xuất đặt hàng ưu tiên — Min-Heap
class AlertService {
private:
    Catalog& catalog;

public:
    AlertService(Catalog& catalog);

    // Lấy các lô sẽ hết hạn trong vòng `days` ngày, sắp theo HSD tăng dần
    int nearExpiry(const Date& today, int days, Batch* results[], int maxResults);
    // Các mặt hàng tồn dưới minStock
    int lowStock(std::string productIds[], int maxResults);
};

#endif // SERVICES_ALERTSERVICE_H
