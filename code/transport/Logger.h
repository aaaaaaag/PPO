//
// Created by denis on 27.08.2022.
//

#ifndef COURSEWORKDB_LOGGER_H
#define COURSEWORKDB_LOGGER_H

#include <string>

namespace polytour::transport {
    class Logger {
    public:

        enum class LogLevel {
            Error = 0,
            Warnings = 1,
            Info = 2,
            Debug = 3,
            Trace = 4
        };

        static void setLogLevel(LogLevel level);

        static void error(const std::string& msg);
        static void warning(const std::string& msg);
        static void info(const std::string& msg);
        static void debug(const std::string& msg);
        static void trace(const std::string& msg);


    };
}

#endif //COURSEWORKDB_LOGGER_H
