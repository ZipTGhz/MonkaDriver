#include "Logger.h"
#include <iostream>

using namespace monkaDriver;

void Logger::Init() {
    std::cout << "[Logger] Initialized" << std::endl;
}

void Logger::Info(const std::string& msg) {
    std::cout << "[INFO] " << msg << std::endl;
}

void Logger::Debug(const std::string& msg) {
    std::cout << "[DEBUG] " << msg << std::endl;
}

void Logger::Error(const std::string& msg) {
    std::cerr << "[ERROR] " << msg << std::endl;
}
