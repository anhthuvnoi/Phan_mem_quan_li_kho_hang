#include "services/ReportService.h"

// Chỉ số Fenwick = Date::toDayIndex() (01/01/2000 -> 1). Ngày ngoài [1, maxDays] bị bỏ qua.

ReportService::ReportService(int maxDays) : revenue(maxDays) {}

void ReportService::addRevenue(const Date& d, double amount) {
    revenue.add(d.toDayIndex(), amount);
}

double ReportService::revenueBetween(const Date& from, const Date& to) const {
    if (to < from) return 0.0;
    return revenue.rangeSum(from.toDayIndex(), to.toDayIndex());
}
