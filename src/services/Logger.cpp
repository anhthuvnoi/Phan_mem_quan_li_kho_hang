#include "services/Logger.h"
#include <ctime>
#include <fstream>

Logger::Logger(const std::string& path) : filePath(path) {}

// Ghi thêm 1 dòng: [yyyy-mm-dd hh:mm:ss] action | detail. Không mở được file thì bỏ qua.
void Logger::log(const std::string& action, const std::string& detail) {
    std::ofstream out(filePath, std::ios::app);
    if (!out) return;
    std::time_t now = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    out << "[" << buf << "] " << action << " | " << detail << "\n";
}
