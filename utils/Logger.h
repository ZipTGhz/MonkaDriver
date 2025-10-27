#pragma once
#include <string>

namespace monkaDriver {
    class Logger {
    public:
        static void Init();
        static void Info(const std::string& msg);
        static void Debug(const std::string& msg);
        static void Error(const std::string& msg);
    };
}
