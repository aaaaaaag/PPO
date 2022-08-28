//
// Created by denis on 05.08.2022.
//

#ifndef COURSEWORKDB_NOTCRITICALERROR_H
#define COURSEWORKDB_NOTCRITICALERROR_H


#include <stdexcept>
#include <string>
#include <utility>
#include "Logger.h"

namespace polytour {
    class NotCriticalError: public std::exception {
    public:
        explicit NotCriticalError(std::string  errorMsg): _errorMsg(std::move(errorMsg)) {
            transport::Logger::warning("Not critical error: " + _errorMsg);
        }

        [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
            return _errorMsg.c_str();
        }
    private:
        std::string _errorMsg;
    };
}



#endif //COURSEWORKDB_NOTCRITICALERROR_H
