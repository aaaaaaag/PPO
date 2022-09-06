//
// Created by denis on 7/23/22.
//

#ifndef COURSEWORKDB_TOURNAMENTPARTICIPANTSREPOSITORY_H
#define COURSEWORKDB_TOURNAMENTPARTICIPANTSREPOSITORY_H

#include "Repository.h"
#include <utility>
#include "TournamentParticipant.h"

namespace polytour::db::repository {
    class TournamentParticipantsRepository: public Repository<transport::TournamentParticipant> {
    public:

        explicit TournamentParticipantsRepository(std::shared_ptr<IConnection> conn):
                Repository<transport::TournamentParticipant>(std::move(conn)) {}

    private:
    protected:
        utility::FieldSet toFieldSet(const transport::TournamentParticipant &obj) override;

        utility::FieldSet toFieldSet(const transport::TournamentParticipant::search_t &obj) override;

        transport::TournamentParticipant fromFieldSet(const utility::FieldSet &field) override;

        Identity getIdentity() override;
    };
}


#endif //COURSEWORKDB_TOURNAMENTPARTICIPANTSREPOSITORY_H
