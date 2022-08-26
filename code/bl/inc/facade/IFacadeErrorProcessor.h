//
// Created by denis on 05.08.2022.
//

#ifndef COURSEWORKDB_IFACADEERRORPROCESSOR_H
#define COURSEWORKDB_IFACADEERRORPROCESSOR_H

#include "User.h"
#include <functional>
#include <optional>
#include "ErrorObj.h"
#include "NotCriticalError.h"
#include "CriticalError.h"

namespace polytour::bl::facade {
    class IFacadeErrorProcessor {
    public:

        virtual ~IFacadeErrorProcessor() = default;

        std::optional<ErrorObj> getError() {
            return _errorObj;
        }

        void clearError() {
            _errorObj = std::nullopt;
        }

        bool isError() {
            return _errorObj.has_value();
        }

    protected:

        void processError(const std::function<void()>& func) {
            try {
                clearError();
                func();
            }
            catch (NotCriticalError& err) {

                _errorObj = ErrorObj(err.what(), ErrorObj::ErrorType::NotCritical);
            }
            catch (CriticalError& err) {

                _errorObj = ErrorObj(err.what(), ErrorObj::ErrorType::Critical);
            }
            catch (...) {

            }
        }

        std::optional<ErrorObj> _errorObj;

    };
}


#endif //COURSEWORKDB_IFACADEERRORPROCESSOR_H
