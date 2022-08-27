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
#include "Logger.h"

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
                transport::Logger::trace(std::string("FacadeErrorProcessor catch not critical error: ") + err.what());
                _errorObj = ErrorObj(err.what(), ErrorObj::ErrorType::NotCritical);
            }
            catch (CriticalError& err) {
                transport::Logger::trace(std::string("FacadeErrorProcessor catch critical error: ") + err.what());
                _errorObj = ErrorObj(err.what(), ErrorObj::ErrorType::Critical);
            }
            catch (...) {
                transport::Logger::error("Occurred unexpected error");
                throw std::runtime_error("Occurred unexpected error");
            }
        }

        std::optional<ErrorObj> _errorObj;

    };
}


#endif //COURSEWORKDB_IFACADEERRORPROCESSOR_H
