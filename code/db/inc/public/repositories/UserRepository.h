//
// Created by denis on 7/12/22.
//

#ifndef COURSEWORKDB_USERREPOSITORY_H
#define COURSEWORKDB_USERREPOSITORY_H

#include "Repository.h"
#include "User.h"
#include <utility>

namespace polytour::db::repository {
    class UserRepository: public Repository<transport::User> {
    public:

        explicit UserRepository(std::shared_ptr<IConnection> conn):
                Repository<transport::User>(std::move(conn)) {}

    private:

        utility::FieldSet toFieldSet(const transport::User &obj) override;

        transport::User fromFieldSet(const utility::FieldSet &field) override;

        utility::FieldSet toFieldSet(const transport::User::search_t &obj) override;

        Identity getIdentity() override;
    };
}


#endif //COURSEWORKDB_USERREPOSITORY_H
