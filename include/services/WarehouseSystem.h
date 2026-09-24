#ifndef SERVICES_WAREHOUSESYSTEM_H
#define SERVICES_WAREHOUSESYSTEM_H

#include "services/Catalog.h"
#include "services/InventoryService.h"
#include "services/OrderService.h"
#include "services/SearchService.h"
#include "services/AlertService.h"
#include "services/ReportService.h"
#include "services/FileManager.h"
#include "services/Logger.h"

// Facade: điểm vào DUY NHẤT cho tầng giao diện (CLI hoặc GUI)
class WarehouseSystem {
private:
    Catalog catalog;
    ReportService report;
    InventoryService inventory;
    OrderService orders;
    SearchService search;
    AlertService alerts;
    FileManager files;
    Logger logger;

public:
    WarehouseSystem();

    Catalog& getCatalog();
    InventoryService& getInventory();
    OrderService& getOrders();
    SearchService& getSearch();
    AlertService& getAlerts();
    ReportService& getReport();

    Result load();
    Result save();
};

#endif // SERVICES_WAREHOUSESYSTEM_H
