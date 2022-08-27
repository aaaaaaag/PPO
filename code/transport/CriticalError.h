//
// Created by denis on 05.08.2022.
//

#ifndef COURSEWORKDB_CRITICALERROR_H
#define COURSEWORKDB_CRITICALERROR_H

#include <stdexcept>
#include <string>
#include <utility>
#include "Logger.h"

namespace polytour {
class CriticalError: public std::exception {
public:

    explicit CriticalError(std::string  errorMsg): _errorMsg(std::move(errorMsg)) {
        polytour::transport::Logger::error("Critical error: " + _errorMsg);
    }

    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return _errorMsg.c_str();
    }

private:

    std::string _errorMsg;

};
}

#endif //COURSEWORKDB_CRITICALERROR_H
