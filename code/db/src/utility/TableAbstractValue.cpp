//
// Created by denis on 11.07.2022.
//

#include "public/utility/TableAbstractValue.h"
#include <stdexcept>
#include "CriticalError.h"

bool polytour::db::utility::TableAbstractValue::isInt() const{
    return _type == TableTypes::Int;
}

bool polytour::db::utility::TableAbstractValue::isString() const{
    return _type == TableTypes::String;
}

bool polytour::db::utility::TableAbstractValue::isNull() const {
    return _type == TableTypes::Null;
}

int polytour::db::utility::TableAbstractValue::toInt() const {
    if (!isInt())
        throw polytour::CriticalError("Failed attempt to cast abstract value to int");
    return std::any_cast<int>(_value);
}

std::string polytour::db::utility::TableAbstractValue::toString() const{
    if (!isString())
        throw polytour::CriticalError("Failed attempt to cast abstract value to string");
    return std::any_cast<std::string>(_value);
}

std::nullopt_t polytour::db::utility::TableAbstractValue::toNull() const {
    if (!isNull())
        throw polytour::CriticalError("Failed attempt to cast abstract value to nullObj");
    return std::any_cast<std::nullopt_t>(_value);
}


