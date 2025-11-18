#include "Logger.h"
#include <iostream>

std::ofstream Logger::logFile;

void Logger::init(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (logFile.is_open()) {
        log("=== Новая сессия запущена ===");
    }
}

std::string Logger::getCurrentTime() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

void Logger::log(const std::string& message) {
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTime() << "] " << message << std::endl;
    }
}

void Logger::close() {
    if (logFile.is_open()) {
        log("=== Сессия завершена ===");
        logFile.close();
    }
}