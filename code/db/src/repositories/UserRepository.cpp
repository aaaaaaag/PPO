//
// Created by denis on 7/12/22.
//

#include "public/repositories/UserRepository.h"
#include "utility/JsonConfiguration.h"

#define ID_KEY "id"
#define NAME_KEY "name"
#define SURNAME_KEY "surname"
#define NICKNAME_KEY "nickname"
#define AGE_KEY "age"
#define EMAIL_KEY "email"
#define PASSWORD_KEY "password"

polytour::db::utility::FieldSet
polytour::db::repository::UserRepository::toFieldSet(const polytour::transport::User &obj) {
    utility::FieldSet result;
    result.addPair(ID_KEY, utility::TableAbstractValue(obj.id));
    result.addPair(NAME_KEY, utility::TableAbstractValue(obj.name));
    if (obj.surname.hasValue())
        result.addPair(SURNAME_KEY, utility::TableAbstractValue(obj.surname.getValue()));
    else
        result.addPair(SURNAME_KEY, utility::TableAbstractValue(obj.surname.toNull()));
    result.addPair(NICKNAME_KEY, utility::TableAbstractValue(obj.nickname));
    if (obj.age.hasValue())
        result.addPair(AGE_KEY, utility::TableAbstractValue(obj.age.getValue()));
    else
        result.addPair(AGE_KEY, utility::TableAbstractValue(obj.age.toNull()));
    result.addPair(EMAIL_KEY, utility::TableAbstractValue(obj.email));
    result.addPair(PASSWORD_KEY, utility::TableAbstractValue(obj.password));
    return result;
}

polytour::transport::User
polytour::db::repository::UserRepository::fromFieldSet(const polytour::db::utility::FieldSet &field) {
    transport::User result;

    if (field.has_value_under_key(ID_KEY))
        result.id = field[ID_KEY].toInt();
    if (field.has_value_under_key(NAME_KEY))
        result.name = field[NAME_KEY].toString();
    if (field.has_value_under_key(SURNAME_KEY)) {
        auto foundValue = field[SURNAME_KEY];
        if (foundValue.isNull())
            result.surname = decltype(result.surname){foundValue.toNull()};
        else
            result.surname = decltype(result.surname){foundValue.toString()};
    }
    if (field.has_value_under_key(NICKNAME_KEY))
        result.nickname = field[NICKNAME_KEY].toString();
    if (field.has_value_under_key(AGE_KEY)) {
        auto foundValue = field[AGE_KEY];
        if (foundValue.isNull())
            result.age = decltype(result.age){foundValue.toNull()};
        else
            result.age = decltype(result.age){foundValue.toInt()};
    }
    if (field.has_value_under_key(EMAIL_KEY))
        result.email = field[EMAIL_KEY].toString();
    if (field.has_value_under_key(PASSWORD_KEY))
        result.password = field[PASSWORD_KEY].toString();
    return result;
}

polytour::db::utility::FieldSet
polytour::db::repository::UserRepository::toFieldSet(const polytour::transport::User::search_t &obj) {
    utility::FieldSet result;
    if (obj.id_.has_value())
        result.addPair(ID_KEY, utility::TableAbstractValue(obj.id_.value()));
    if (obj.name_.has_value())
        result.addPair(NAME_KEY, utility::TableAbstractValue(obj.name_.value()));
    if (obj.surname_.has_value()) {
        auto surname = obj.surname_.value();
        if (surname.isNull())
            result.addPair(SURNAME_KEY, utility::TableAbstractValue(surname.toNull()));
        else
            result.addPair(SURNAME_KEY, utility::TableAbstractValue(surname.getValue()));
    }
    if (obj.nickname_.has_value())
        result.addPair(NICKNAME_KEY, utility::TableAbstractValue(obj.nickname_.value()));
    if (obj.age_.has_value()) {
        auto age = obj.age_.value();
        if (age.isNull())
            result.addPair(AGE_KEY, utility::TableAbstractValue(age.toNull()));
        else
            result.addPair(AGE_KEY, utility::TableAbstractValue(age.getValue()));
    }
    if (obj.email_.has_value())
        result.addPair(EMAIL_KEY, utility::TableAbstractValue(obj.email_.value()));
    if (obj.password_.has_value())
        result.addPair(PASSWORD_KEY, utility::TableAbstractValue(obj.password_.value()));
    return result;
}

polytour::db::repository::Identity polytour::db::repository::UserRepository::getIdentity() {
    Identity result;

    result.tableName = utility::JsonConfiguration::getInstance()->table_users_name;
    result.tableColumns.emplace(ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Serial,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(NAME_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(SURNAME_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(NICKNAME_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(AGE_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(EMAIL_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(PASSWORD_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::NotNull
    });
    return result;
}