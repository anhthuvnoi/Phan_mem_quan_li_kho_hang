#ifndef SERVICES_RESULT_H
#define SERVICES_RESULT_H

#include <string>

// Kết quả trả về cho tầng giao diện (không in trực tiếp trong service)
struct Result {
    bool ok;
    std::string message;
    Result(bool ok = true, const std::string& msg = "") : ok(ok), message(msg) {}
};

#endif // SERVICES_RESULT_H
