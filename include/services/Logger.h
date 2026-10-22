#ifndef SERVICES_LOGGER_H
#define SERVICES_LOGGER_H

#include <string>

// Ghi log thao tác ra data/log.txt
class Logger {
private:
    std::string filePath;

public:
    Logger(const std::string& path = "data/log.txt");
    void log(const std::string& action, const std::string& detail);
};

#endif // SERVICES_LOGGER_H
