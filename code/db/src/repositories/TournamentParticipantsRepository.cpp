//
// Created by denis on 7/23/22.
//

#include "repositories/TournamentParticipantsRepository.h"
#include "utility/JsonConfiguration.h"

#define ID_KEY "id"
#define TOURNAMENT_ID_KEY "tournament_id"
#define PARTICIPANT_ID_KEY "participant_id"

polytour::db::utility::FieldSet polytour::db::repository::TournamentParticipantsRepository::toFieldSet(
        const polytour::transport::TournamentParticipant &obj) {
    utility::FieldSet result;
    result.addPair(ID_KEY, utility::TableAbstractValue(obj.id));
    result.addPair(TOURNAMENT_ID_KEY, utility::TableAbstractValue(obj.tournament_id));
    result.addPair(PARTICIPANT_ID_KEY, utility::TableAbstractValue(obj.participant_id));
    return result;
}

polytour::db::utility::FieldSet polytour::db::repository::TournamentParticipantsRepository::toFieldSet(
        const polytour::transport::TournamentParticipant::search_t &obj) {
    utility::FieldSet result;
    if (obj.id_.has_value())
        result.addPair(ID_KEY, utility::TableAbstractValue(obj.id_.value()));
    if (obj.tournament_id_.has_value())
        result.addPair(TOURNAMENT_ID_KEY, utility::TableAbstractValue(obj.tournament_id_.value()));
    if (obj.participant_id_.has_value())
        result.addPair(PARTICIPANT_ID_KEY, utility::TableAbstractValue(obj.participant_id_.value()));
    return result;
}

polytour::transport::TournamentParticipant
polytour::db::repository::TournamentParticipantsRepository::fromFieldSet(const polytour::db::utility::FieldSet &field) {
    transport::TournamentParticipant result;

    if (field.has_value_under_key(ID_KEY))
        result.id = field[ID_KEY].toInt();
    if (field.has_value_under_key(TOURNAMENT_ID_KEY))
        result.tournament_id = field[TOURNAMENT_ID_KEY].toInt();
    if (field.has_value_under_key(PARTICIPANT_ID_KEY))
        result.participant_id = field[PARTICIPANT_ID_KEY].toInt();
    return result;
}

polytour::db::repository::Identity polytour::db::repository::TournamentParticipantsRepository::getIdentity() {
    Identity result;

    result.tableName = utility::JsonConfiguration::getInstance()->table_tournament_participants_name;
    result.tableColumns.emplace(ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Serial,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(TOURNAMENT_ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::NotNull
    });
    result.tableColumns.emplace(PARTICIPANT_ID_KEY, Identity::column_description_t{
            .type = utility::TableTypes::Int,
            .mandatory = utility::FieldMandatory::NotNull
    });
    return result;
}
