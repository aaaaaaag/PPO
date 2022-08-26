//
// Created by denis on 14.07.2022.
//

#ifndef COURSEWORKDB_TOURNAMENTREPOSITORY_H
#define COURSEWORKDB_TOURNAMENTREPOSITORY_H

#include "Repository.h"
#include <utility>
#include "Tournament.h"

namespace polytour::db::repository {
    class TournamentRepository: public Repository<transport::Tournament> {
    public:

        explicit TournamentRepository(std::shared_ptr<IConnection> conn):
                Repository<transport::Tournament>(std::move(conn)) {}

    private:

        utility::FieldSet toFieldSet(const transport::Tournament &obj) override;

        transport::Tournament fromFieldSet(const utility::FieldSet &field) override;

        utility::FieldSet toFieldSet(const transport::Tournament::search_t &obj) override;

        Identity getIdentity() override;
    };
}



#endif //COURSEWORKDB_TOURNAMENTREPOSITORY_H
