//
// Created by denis on 19.07.2022.
//

#include "public/repositories/MatchRepository.h"

#define ID_KEY "id"
#define TOURNAMENT_ID_KEY "tournament_id"
#define PARTICIPANT_1_ID_KEY "participant_1_id"
#define PARTICIPANT_2_ID_KEY "participant_2_id"
#define PARTICIPANT_1_TRIBE_KEY "participant_1_tribe"
#define PARTICIPANT_2_TRIBE_KEY "participant_2_tribe"
#define WINNER_ID_KEY "winner_id"
#define LOSER_ID_KEY "loser_id"
#define STATUS_KEY "status"
#define NEXT_MATCH_ID_KEY "next_match_id"
#define PREV_MATCH_1_ID "prev_match_1_id"
#define PREV_MATCH_2_ID "prev_match_2_id"
#define TOUR_KEY "tour"

polytour::db::utility::FieldSet
polytour::db::repository::MatchRepository::toFieldSet(const polytour::transport::Match &obj) {
    utility::FieldSet result;
    result.addPair(ID_KEY, utility::TableAbstractValue(obj.id));
    result.addPair(TOURNAMENT_ID_KEY, utility::TableAbstractValue(obj.tournament_id));
    result.addPair(TOUR_KEY, utility::TableAbstractValue(obj.tour));
    result.addPair(STATUS_KEY, utility::TableAbstractValue(obj.status));
    if (obj.participant_1_id.hasValue())
        result.addPair(PARTICIPANT_1_ID_KEY, utility::TableAbstractValue(obj.participant_1_id.getValue()));
    else
        result.addPair(PARTICIPANT_1_ID_KEY, utility::TableAbstractValue(obj.participant_1_id.toNull()));
    if (obj.participant_2_id.hasValue())
        result.addPair(PARTICIPANT_2_ID_KEY, utility::TableAbstractValue(obj.participant_2_id.getValue()));
    else
        result.addPair(PARTICIPANT_2_ID_KEY, utility::TableAbstractValue(obj.participant_2_id.toNull()));
    if (obj.participant_1_tribe.hasValue())
        result.addPair(PARTICIPANT_1_TRIBE_KEY, utility::TableAbstractValue(obj.participant_1_tribe.getValue()));
    else
        result.addPair(PARTICIPANT_1_TRIBE_KEY, utility::TableAbstractValue(obj.participant_1_tribe.toNull()));
    if (obj.participant_2_tribe.hasValue())
        result.addPair(PARTICIPANT_2_TRIBE_KEY, utility::TableAbstractValue(obj.participant_2_tribe.getValue()));
    else
        result.addPair(PARTICIPANT_2_TRIBE_KEY, utility::TableAbstractValue(obj.participant_2_tribe.toNull()));
    if (obj.winner_id.hasValue())
        result.addPair(WINNER_ID_KEY, utility::TableAbstractValue(obj.winner_id.getValue()));
    else
        result.addPair(WINNER_ID_KEY, utility::TableAbstractValue(obj.winner_id.toNull()));
    if (obj.loser_id.hasValue())
        result.addPair(LOSER_ID_KEY, utility::TableAbstractValue(obj.loser_id.getValue()));
    else
        result.addPair(LOSER_ID_KEY, utility::TableAbstractValue(obj.loser_id.toNull()));
    if (obj.next_match_id.hasValue())
        result.addPair(NEXT_MATCH_ID_KEY, utility::TableAbstractValue(obj.next_match_id.getValue()));
    else
        result.addPair(NEXT_MATCH_ID_KEY, utility::TableAbstractValue(obj.next_match_id.toNull()));
    if (obj.prev_match_1_id.hasValue())
        result.addPair(PREV_MATCH_1_ID, utility::TableAbstractValue(obj.prev_match_1_id.getValue()));
    else
        result.addPair(PREV_MATCH_1_ID, utility::TableAbstractValue(obj.prev_match_1_id.toNull()));
    if (obj.prev_match_2_id.hasValue())
        result.addPair(PREV_MATCH_2_ID, utility::TableAbstractValue(obj.prev_match_2_id.getValue()));
    else
        result.addPair(PREV_MATCH_2_ID, utility::TableAbstractValue(obj.prev_match_2_id.toNull()));
    return result;
}


polytour::db::utility::FieldSet
polytour::db::repository::MatchRepository::toFieldSet(const polytour::transport::Match::search_t &obj) {
    utility::FieldSet result;
    if (obj.id_.has_value())
        result.addPair(ID_KEY, utility::TableAbstractValue(obj.id_.value()));
    if (obj.tournament_id_.has_value())
        result.addPair(TOURNAMENT_ID_KEY, utility::TableAbstractValue(obj.tournament_id_.value()));
    if (obj.tour_.has_value())
        result.addPair(TOUR_KEY, utility::TableAbstractValue(obj.tour_.value()));
    if (obj.status_.has_value())
        result.addPair(STATUS_KEY, utility::TableAbstractValue(obj.status_.value()));
    if (obj.participant_1_id_.has_value()) {
        auto value = obj.participant_1_id_.value();
        if (value.isNull()) result.addPair(PARTICIPANT_1_ID_KEY, utility::TableAbstractValue(value.toNull()));
        else result.addPair(PARTICIPANT_1_ID_KEY, utility::TableAbstractValue(value.getValue()));
    }
    if (obj.participant_2_id_.has_value()) {
        auto value = obj.participant_2_id_.value();
        if (value.isNull()) result.addPair(PARTICIPANT_2_ID_KEY, utility::TableAbstractValue(value.toNull()));
        else result.addPair(PARTICIPANT_2_ID_KEY, utility::TableAbstractValue(value.getValue()));
    }
    if (obj.participant_1_tribe_.has_value()) {
        auto value = obj.participant_1_tribe_.value();
        if (value.isNull()) result.addPair(PARTICIPANT_1_TRIBE_KEY, utility::TableAbstractValue(value.toNull()));
        else result.addPair(PARTICIPANT_1_TRIBE_KEY, utility::TableAbstractValue(value.getValue()));
    }
    if (obj.participant_2_tribe_.has_value()) {
        auto value = obj.participant_2_tribe_.value();
        if (value.isNull()) result.addPair(PARTICIPANT_2_TRIBE_KEY, utility::TableAbstractValue(value.toNull()));
        else result.addPair(PARTICIPANT_2_TRIBE_KEY, utility::TableAbstractValue(value.getValue()));
    }
    if (obj.winner_id_.has_value()) {
        auto value = obj.winner_id_.value();
        if (value.isNull()) result.addPair(WINNER_ID_KEY, utility::TableAbstractValue(value.toNull()));
        else result.addPair(WINNER_ID_KEY, utility::TableAbstractValue(value.getValue()));
    }
    if (obj.loser_id_.has_value()) {
        auto value = obj.loser_id_.value();
        if (value.isNull()) result.addPair(LOSER_ID_KEY, utility::TableAbstractValue(value.toNull()));
        else result.addPair(LOSER_ID_KEY, utility::TableAbstractValue(value.getValue()));
    }
    if (obj.next_match_id_.has_value()) {
        auto value = obj.next_match_id_.value();
        if (value.isNull()) result.addPair(NEXT_MATCH_ID_KEY, utility::TableAbstractValue(value.toNull()));
        else result.addPair(NEXT_MATCH_ID_KEY, utility::TableAbstractValue(value.getValue()));
    }
    if (obj.prev_match_1_id_.has_value()) {
        auto value = obj.prev_match_1_id_.value();
        if (value.isNull()) result.addPair(PREV_MATCH_1_ID, utility::TableAbstractValue(value.toNull()));
        else result.addPair(PREV_MATCH_1_ID, utility::TableAbstractValue(value.getValue()));
    }
    if (obj.prev_match_2_id_.has_value()) {
        auto value = obj.prev_match_2_id_.value();
        if (value.isNull()) result.addPair(PREV_MATCH_2_ID, utility::TableAbstractValue(value.toNull()));
        else result.addPair(PREV_MATCH_2_ID, utility::TableAbstractValue(value.getValue()));
    }
    return result;
}

polytour::transport::Match
polytour::db::repository::MatchRepository::fromFieldSet(const polytour::db::utility::FieldSet &field) {
    transport::Match result;

    if (field.has_value_under_key(ID_KEY))
        result.id = field[ID_KEY].toInt();
    if (field.has_value_under_key(TOURNAMENT_ID_KEY))
        result.tournament_id = field[TOURNAMENT_ID_KEY].toInt();
    if (field.has_value_under_key(STATUS_KEY))
        result.status = field[STATUS_KEY].toString();
    if (field.has_value_under_key(TOUR_KEY))
        result.tour = field[TOUR_KEY].toInt();
    if (field.has_value_under_key(PARTICIPANT_1_ID_KEY)) {
        auto foundValue = field[PARTICIPANT_1_ID_KEY];
        if (foundValue.isNull()) result.participant_1_id = decltype(result.participant_1_id){foundValue.toNull()};
        else result.participant_1_id = decltype(result.participant_1_id){foundValue.toInt()};
    }
    if (field.has_value_under_key(PARTICIPANT_2_ID_KEY)) {
        auto foundValue = field[PARTICIPANT_2_ID_KEY];
        if (foundValue.isNull()) result.participant_2_id = decltype(result.participant_2_id){foundValue.toNull()};
        else result.participant_2_id = decltype(result.participant_2_id){foundValue.toInt()};
    }
    if (field.has_value_under_key(PARTICIPANT_1_TRIBE_KEY)) {
        auto foundValue = field[PARTICIPANT_1_TRIBE_KEY];
        if (foundValue.isNull()) result.participant_1_tribe = decltype(result.participant_1_tribe){foundValue.toNull()};
        else result.participant_1_tribe = decltype(result.participant_1_tribe){foundValue.toString()};
    }
    if (field.has_value_under_key(PARTICIPANT_2_TRIBE_KEY)) {
        auto foundValue = field[PARTICIPANT_2_TRIBE_KEY];
        if (foundValue.isNull()) result.participant_2_tribe = decltype(result.participant_2_tribe){foundValue.toNull()};
        else result.participant_2_tribe = decltype(result.participant_2_tribe){foundValue.toString()};
    }
    if (field.has_value_under_key(WINNER_ID_KEY)) {
        auto foundValue = field[WINNER_ID_KEY];
        if (foundValue.isNull()) result.winner_id = decltype(result.winner_id){foundValue.toNull()};
        else result.winner_id = decltype(result.winner_id){foundValue.toInt()};
    }
    if (field.has_value_under_key(LOSER_ID_KEY)) {
        auto foundValue = field[LOSER_ID_KEY];
        if (foundValue.isNull()) result.loser_id = decltype(result.loser_id){foundValue.toNull()};
        else result.loser_id = decltype(result.loser_id){foundValue.toInt()};
    }
    if (field.has_value_under_key(NEXT_MATCH_ID_KEY)) {
        auto foundValue = field[NEXT_MATCH_ID_KEY];
        if (foundValue.isNull()) result.next_match_id = decltype(result.next_match_id){foundValue.toNull()};
        else result.next_match_id = decltype(result.next_match_id){foundValue.toInt()};
    }
    if (field.has_value_under_key(PREV_MATCH_1_ID)) {
        auto foundValue = field[PREV_MATCH_1_ID];
        if (foundValue.isNull()) result.prev_match_1_id = decltype(result.prev_match_1_id){foundValue.toNull()};
        else result.prev_match_1_id = decltype(result.prev_match_1_id){foundValue.toInt()};
    }
    if (field.has_value_under_key(PREV_MATCH_2_ID)) {
        auto foundValue = field[PREV_MATCH_2_ID];
        if (foundValue.isNull()) result.prev_match_2_id = decltype(result.prev_match_2_id){foundValue.toNull()};
        else result.prev_match_2_id = decltype(result.prev_match_2_id){foundValue.toInt()};
    }
    return result;
}

polytour::db::repository::Identity polytour::db::repository::MatchRepository::getIdentity() {
    Identity result;

    result.tableName = "matches";
    result.tableColumns.emplace(ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Serial,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(TOURNAMENT_ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(STATUS_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(PARTICIPANT_1_ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(PARTICIPANT_2_ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(PARTICIPANT_1_TRIBE_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(PARTICIPANT_2_TRIBE_KEY, Identity::column_description_t{
            .type = utility::TableTypes::String,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(WINNER_ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(LOSER_ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(NEXT_MATCH_ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(PREV_MATCH_1_ID, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(PREV_MATCH_2_ID, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::MayBeNull
    });
    result.tableColumns.emplace(TOUR_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::NotNull
    });

    return result;
}
