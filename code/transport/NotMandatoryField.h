//
// Created by denis on 7/22/22.
//

#ifndef COURSEWORKDB_NOTMANDATORYFIELD_H
#define COURSEWORKDB_NOTMANDATORYFIELD_H


#include <optional>
#include <stdexcept>
#include "CriticalError.h"

namespace polytour::transport::utility {
    template <class T>
    class NotMandatoryField {
    public:
        using value_t = T;
        NotMandatoryField(): _value(std::nullopt){};
        explicit NotMandatoryField(const T& value): _value(value){};
        explicit NotMandatoryField(T&& value): _value(value){};
        explicit NotMandatoryField(std::nullopt_t null):_value(null){};

        [[nodiscard]] bool hasValue() const {return _value.has_value();}
        [[nodiscard]] bool isNull() const {return !_value.has_value();}
        [[nodiscard]] T getValue() const {
            if (!hasValue())
                throw polytour::CriticalError("Attempt to get value from null");
            return _value.value();
        }
        [[nodiscard]] std::nullopt_t toNull() const {
            if (!isNull())
                throw polytour::CriticalError("Attempt to get null from initialised field");
            return std::nullopt;
        }

        NotMandatoryField<T>& operator= (const T& value) {
            _value = value;
            return *this;
        }

    private:
        std::optional<T> _value;
    };
}

#endif //COURSEWORKDB_NOTMANDATORYFIELD_H
