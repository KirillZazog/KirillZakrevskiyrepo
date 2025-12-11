#ifndef LOGGER_H
#define LOGGER_H
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <fstream>
#include <ctime>

class Logger {
private:
    static std::ofstream logFile;
    static std::string getCurrentTime();

public:
    static void init(const std::string& filename = "pipeline.log");
    static void log(const std::string& message);
    static void close();
};

#endif