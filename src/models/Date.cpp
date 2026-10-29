#include "models/Date.h"

// File mẫu đã cài đặt đầy đủ — tham khảo cách viết cho các file khác

Date::Date() : day(1), month(1), year(2000) {}
Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

int Date::getDay() const { return day; }
int Date::getMonth() const { return month; }
int Date::getYear() const { return year; }

bool Date::isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::daysInMonth(int m, int y) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeapYear(y)) return 29;
    return days[m - 1];
}

bool Date::isValid() const {
    if (year < 2000 || month < 1 || month > 12) return false;
    return day >= 1 && day <= daysInMonth(month, year);
}

int Date::toDayIndex() const {
    int idx = 0;
    for (int y = 2000; y < year; ++y) idx += isLeapYear(y) ? 366 : 365;
    for (int m = 1; m < month; ++m) idx += daysInMonth(m, year);
    return idx + day;   // 01/01/2000 -> 1 (Fenwick bắt đầu từ 1)
}

int Date::operator-(const Date& other) const { return toDayIndex() - other.toDayIndex(); }

bool Date::operator<(const Date& o) const {
    if (year != o.year) return year < o.year;
    if (month != o.month) return month < o.month;
    return day < o.day;
}
bool Date::operator==(const Date& o) const {
    return day == o.day && month == o.month && year == o.year;
}
bool Date::operator<=(const Date& o) const { return *this < o || *this == o; }

static std::string pad2(int x) { return (x < 10 ? "0" : "") + std::to_string(x); }

std::string Date::toString() const {
    return pad2(day) + "/" + pad2(month) + "/" + std::to_string(year);
}

Date Date::fromString(const std::string& s) {
    // định dạng dd/mm/yyyy — tự tách chuỗi, không dùng thư viện parse
    int parts[3] = {0, 0, 0}, k = 0;
    for (size_t i = 0; i < s.size() && k < 3; ++i) {
        if (s[i] == '/') { ++k; continue; }
        if (s[i] >= '0' && s[i] <= '9') parts[k] = parts[k] * 10 + (s[i] - '0');
    }
    return Date(parts[0], parts[1], parts[2]);
}
