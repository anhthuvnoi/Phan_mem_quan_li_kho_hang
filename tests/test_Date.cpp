#include <cassert>
#include <iostream>
#include "models/Date.h"

int main() {
    Date a(1, 1, 2000), b(1, 3, 2024), c(28, 2, 2024);
    assert(a.toDayIndex() == 1);
    assert(Date::isLeapYear(2024) && !Date::isLeapYear(2100));
    assert(b - c == 2);                       // 2024 nhuận: 28/2 -> 29/2 -> 1/3
    assert(c < b && !(b < c));
    assert(Date::fromString("05/09/2026") == Date(5, 9, 2026));
    assert(Date(5, 9, 2026).toString() == "05/09/2026");
    assert(!Date(31, 4, 2026).isValid());
    std::cout << "test_Date: OK\n";
    return 0;
}
