//
// Created by denis on 7/22/22.
//

#ifndef COURSEWORKDB_TOURNAMENT_H
#define COURSEWORKDB_TOURNAMENT_H


#include "IIdentifiable.h"
#include "NotMandatoryField.h"
#include <string>
#include <optional>

namespace polytour::transport {
    class Tournament: public IIdentifiable{
    public:
        int id;
        std::string name;
        utility::NotMandatoryField<std::string> description;
        std::string status;
        int organizer_id = -1;
        int max_participants_num = -1;
        int cur_participants_num = -1;
        int getOID() override { return id; }

        static std::string status_wait_for_participants() { return "wait"; };
        static std::string status_started() { return "started"; };
        static std::string status_finished() { return "finished"; };

        struct SearchTournament{
            std::optional<decltype(id)> id_;
            std::optional<decltype(name)> name_;
            std::optional<decltype(description)> description_;
            std::optional<decltype(status)> status_;
            std::optional<decltype(organizer_id)> organizer_id_;
            std::optional<decltype(max_participants_num)> max_participants_num_;
            std::optional<decltype(cur_participants_num)> cur_participants_num_;
        };

        using search_t = SearchTournament;
    };
}

#endif //COURSEWORKDB_TOURNAMENT_H
