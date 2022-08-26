//
// Created by denis on 7/22/22.
//

#ifndef COURSEWORKDB_MATCH_H
#define COURSEWORKDB_MATCH_H

#include "IIdentifiable.h"
#include <string>
#include <optional>
#include "NotMandatoryField.h"

namespace polytour::transport {
    class Match: public IIdentifiable{
    public:
        int id;
        int tournament_id;
        std::string status;
        utility::NotMandatoryField<int> participant_1_id;
        utility::NotMandatoryField<int> participant_2_id;
        utility::NotMandatoryField<std::string> participant_1_tribe;
        utility::NotMandatoryField<std::string> participant_2_tribe;
        utility::NotMandatoryField<int> winner_id;
        utility::NotMandatoryField<int> loser_id;
        utility::NotMandatoryField<int> next_match_id;
        utility::NotMandatoryField<int> prev_match_1_id;
        utility::NotMandatoryField<int> prev_match_2_id;
        int tour;

        static std::string status_wait_participants(){ return "wait_participants"; }
        static std::string status_wait_tribes() { return "wait_tribes"; }
        static std::string status_started() { return "started"; }
        static std::string status_finished() { return "finished"; };

        int getOID() override { return id; }

        struct SearchMatch{
            std::optional<decltype(id)> id_;
            std::optional<decltype(tournament_id)> tournament_id_;
            std::optional<decltype(participant_1_id)> participant_1_id_;
            std::optional<decltype(participant_2_id)> participant_2_id_;
            std::optional<decltype(participant_1_tribe)> participant_1_tribe_;
            std::optional<decltype(participant_2_tribe)> participant_2_tribe_;
            std::optional<decltype(winner_id)> winner_id_;
            std::optional<decltype(loser_id)> loser_id_;
            std::optional<decltype(status)> status_;
            std::optional<decltype(next_match_id)> next_match_id_;
            std::optional<decltype(prev_match_1_id)> prev_match_1_id_;
            std::optional<decltype(prev_match_2_id)> prev_match_2_id_;
            std::optional<decltype(tour)> tour_;
        };

        using search_t = SearchMatch;
    };
}

#endif //COURSEWORKDB_MATCH_H
