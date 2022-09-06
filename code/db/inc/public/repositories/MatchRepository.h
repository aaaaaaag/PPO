//
// Created by denis on 19.07.2022.
//

#ifndef COURSEWORKDB_MATCHREPOSITORY_H
#define COURSEWORKDB_MATCHREPOSITORY_H

#include "Repository.h"
#include <utility>
#include "Match.h"

namespace polytour::db::repository {
    class MatchRepository: public Repository<transport::Match> {
    public:

        explicit MatchRepository(std::shared_ptr<IConnection> conn):
                Repository<transport::Match>(std::move(conn)) {}

    protected:
        utility::FieldSet toFieldSet(const transport::Match &obj) override;

        utility::FieldSet toFieldSet(const transport::Match::search_t &obj) override;

        transport::Match fromFieldSet(const utility::FieldSet &field) override;

        Identity getIdentity() override;

    };
}

#endif //COURSEWORKDB_MATCHREPOSITORY_H
