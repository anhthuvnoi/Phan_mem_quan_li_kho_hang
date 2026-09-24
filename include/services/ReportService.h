#ifndef SERVICES_REPORTSERVICE_H
#define SERVICES_REPORTSERVICE_H

#include "ds/FenwickTree.h"
#include "models/Date.h"

// Báo cáo doanh thu theo khoảng ngày — Fenwick Tree
class ReportService {
private:
    FenwickTree revenue;

public:
    ReportService(int maxDays = 20000);   // ~ 55 năm tính từ 01/01/2000

    void addRevenue(const Date& d, double amount);
    double revenueBetween(const Date& from, const Date& to) const;
};

#endif // SERVICES_REPORTSERVICE_H
