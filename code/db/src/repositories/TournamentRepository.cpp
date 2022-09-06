//
// Created by denis on 14.07.2022.
//

#include "public/repositories/TournamentRepository.h"
#include "utility/JsonConfiguration.h"

#define ID_KEY "id"
#define NAME_KEY "name"
#define DESCRIPTION_KEY "description"
#define STATUS_KEY "status"
#define ORGANIZER_ID_KEY "organizer_id"
#define MAX_PARTICIPANTS_KEY "max_participants"
#define CUR_PARTICIPANTS_KEY "cur_participants"

polytour::db::utility::FieldSet
polytour::db::repository::TournamentRepository::toFieldSet(const polytour::transport::Tournament &obj) {
    utility::FieldSet result;
    result.addPair(ID_KEY, utility::TableAbstractValue(obj.id));
    result.addPair(NAME_KEY, utility::TableAbstractValue(obj.name));
    if (obj.description.isNull())
        result.addPair(DESCRIPTION_KEY, utility::TableAbstractValue(obj.description.toNull()));
    else
        result.addPair(DESCRIPTION_KEY, utility::TableAbstractValue(obj.description.getValue()));
    result.addPair(STATUS_KEY, utility::TableAbstractValue(obj.status));
    result.addPair(ORGANIZER_ID_KEY, utility::TableAbstractValue(obj.organizer_id));
    result.addPair(MAX_PARTICIPANTS_KEY, utility::TableAbstractValue(obj.max_participants_num));
    result.addPair(CUR_PARTICIPANTS_KEY, utility::TableAbstractValue(obj.cur_participants_num));
    return result;
}

polytour::transport::Tournament
polytour::db::repository::TournamentRepository::fromFieldSet(const polytour::db::utility::FieldSet &field) {
    transport::Tournament result;

    if (field.has_value_under_key(ID_KEY))
        result.id = field[ID_KEY].toInt();
    if (field.has_value_under_key(NAME_KEY))
        result.name = field[NAME_KEY].toString();
    if (field.has_value_under_key(DESCRIPTION_KEY)) {
        auto description = field[DESCRIPTION_KEY];
        if (description.isNull())
            result.description = decltype(result.description){description.toNull()};
        else
            result.description = decltype(result.description){description.toString()};
    }
    if (field.has_value_under_key(STATUS_KEY))
        result.status = field[STATUS_KEY].toString();
    if (field.has_value_under_key(ORGANIZER_ID_KEY))
        result.organizer_id = field[ORGANIZER_ID_KEY].toInt();
    if (field.has_value_under_key(MAX_PARTICIPANTS_KEY))
        result.max_participants_num = field[MAX_PARTICIPANTS_KEY].toInt();
    if (field.has_value_under_key(CUR_PARTICIPANTS_KEY))
        result.cur_participants_num = field[CUR_PARTICIPANTS_KEY].toInt();
    return result;
}

polytour::db::utility::FieldSet
polytour::db::repository::TournamentRepository::toFieldSet(const polytour::transport::Tournament::search_t &obj) {
    utility::FieldSet result;
    if (obj.id_.has_value())
        result.addPair(ID_KEY, utility::TableAbstractValue(obj.id_.value()));
    if (obj.name_.has_value())
        result.addPair(NAME_KEY, utility::TableAbstractValue(obj.name_.value()));
    if (obj.description_.has_value()) {
        auto description = obj.description_.value();
        if (description.isNull())
            result.addPair(DESCRIPTION_KEY, utility::TableAbstractValue(description.toNull()));
        else
            result.addPair(DESCRIPTION_KEY, utility::TableAbstractValue(description.getValue()));
    }
    if (obj.status_.has_value())
        result.addPair(STATUS_KEY, utility::TableAbstractValue(obj.status_.value()));
    if (obj.organizer_id_.has_value())
        result.addPair(ORGANIZER_ID_KEY, utility::TableAbstractValue(obj.organizer_id_.value()));
    if (obj.max_participants_num_.has_value())
        result.addPair(MAX_PARTICIPANTS_KEY, utility::TableAbstractValue(obj.max_participants_num_.value()));
    if (obj.cur_participants_num_.has_value())
        result.addPair(CUR_PARTICIPANTS_KEY, utility::TableAbstractValue(obj.cur_participants_num_.value()));
    return result;
}

polytour::db::repository::Identity polytour::db::repository::TournamentRepository::getIdentity() {
    Identity result;

    result.tableName = utility::JsonConfiguration::getInstance()->table_tournaments_name;
    result.tableColumns.emplace(ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Serial,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(NAME_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(DESCRIPTION_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(STATUS_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::NotNull
    });result.tableColumns.emplace(ORGANIZER_ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::NotNull
    });result.tableColumns.emplace(MAX_PARTICIPANTS_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(CUR_PARTICIPANTS_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::NotNull
    });
    return result;
}
