//
// Created by denis on 05.08.2022.
//

#include "ErrorObj.h"
#include <utility>

polytour::bl::ErrorObj::ErrorObj(std::string errorMsg, ErrorType errorType):
_errorMessage(std::move(errorMsg)),
_errorType(errorType){}

std::string polytour::bl::ErrorObj::getErrorMessage() {
    return _errorMessage;
}

polytour::bl::ErrorObj::ErrorType polytour::bl::ErrorObj::getErrorType() {
    return _errorType;
}
