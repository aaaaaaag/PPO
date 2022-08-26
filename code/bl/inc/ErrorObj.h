//
// Created by denis on 05.08.2022.
//

#ifndef COURSEWORKDB_ERROROBJ_H
#define COURSEWORKDB_ERROROBJ_H

#include <string>

namespace polytour::bl {
    class ErrorObj {
    public:

        enum class ErrorType {
            Critical = 0,
            NotCritical
        };

        explicit ErrorObj(std::string errorMsg, ErrorType errorType);

        std::string getErrorMessage();

        ErrorType getErrorType();

    private:
        std::string _errorMessage;
        ErrorType _errorType;
    };
}

#endif //COURSEWORKDB_ERROROBJ_H
