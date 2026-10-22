#ifndef MODELS_DATE_H
#define MODELS_DATE_H

#include <string>

class Date {
private:
    int day, month, year;

public:
    Date();
    Date(int d, int m, int y);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    bool isValid() const;
    static bool isLeapYear(int y);
    static int daysInMonth(int m, int y);

    int toDayIndex() const;                  // số ngày kể từ 01/01/2000 (dùng cho Fenwick)
    int operator-(const Date& other) const;  // số ngày chênh lệch

    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator<=(const Date& other) const;

    std::string toString() const;                    // "dd/mm/yyyy"
    static Date fromString(const std::string& s);
};

#endif // MODELS_DATE_H
