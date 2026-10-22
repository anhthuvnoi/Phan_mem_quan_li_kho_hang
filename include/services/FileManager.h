#ifndef SERVICES_FILEMANAGER_H
#define SERVICES_FILEMANAGER_H

#include <string>
#include "services/Catalog.h"
#include "services/InventoryService.h"
#include "services/Result.h"

// Nạp / lưu dữ liệu từ thư mục data/
class FileManager {
private:
    std::string dataDir;

public:
    FileManager(const std::string& dataDir = "data/");

    Result loadAll(Catalog& catalog, InventoryService& inv);
    Result saveAll(Catalog& catalog, InventoryService& inv);
};

#endif // SERVICES_FILEMANAGER_H
