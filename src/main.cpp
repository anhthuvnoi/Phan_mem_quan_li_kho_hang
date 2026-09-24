#include <iostream>
#include "models/Date.h"

// Tạm thời: menu dòng lệnh (CLI). Giao diện đồ hoạ viết trong thư mục ui/
// và chỉ gọi qua services/WarehouseSystem.h
int main() {
    Date today(24, 9, 2026);
    std::cout << "Warehouse Management - PBL2\n";
    std::cout << "Hom nay: " << today.toString() << "\n";
    return 0;
}
