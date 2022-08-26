//
// Created by denis on 11.07.2022.
//

#ifndef COURSEWORKDB_TABLEABSTRACTVALUE_H
#define COURSEWORKDB_TABLEABSTRACTVALUE_H

#include "BaseDefines.h"
#include <any>
#include <optional>

namespace polytour {
namespace db {
namespace utility {
    class TableAbstractValue
    {
    public:
        template<class T>
        TableAbstractValue(const T& value, TableTypes type) :
            _value(value),
            _type(type){}

        explicit TableAbstractValue(const std::string& str):
                TableAbstractValue(str, TableTypes::String){};

        explicit TableAbstractValue(int value):
                TableAbstractValue(value, TableTypes::Int){};

        explicit TableAbstractValue(std::nullopt_t value):
                TableAbstractValue(value, TableTypes::Null){};

        [[nodiscard]] bool isInt() const;
        [[nodiscard]] bool isString() const;
        [[nodiscard]] bool isNull() const;

        [[nodiscard]] int toInt() const;
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] std::nullopt_t toNull() const;

    private:
        TableTypes _type;
        std::any _value;
    };
}
}
}


#endif //COURSEWORKDB_TABLEABSTRACTVALUE_H
