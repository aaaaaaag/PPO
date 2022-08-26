//
// Created by denis on 7/23/22.
//

#ifndef COURSEWORKDB_IUSERTRANSACTIONFACTORY_H
#define COURSEWORKDB_IUSERTRANSACTIONFACTORY_H

#include "User.h"
#include <vector>

namespace polytour::bl::transaction {
    class IUserTransactionFactory {
    public:

        virtual ~IUserTransactionFactory() = default;

        virtual void addUser(const transport::User& user) = 0;

        virtual void deleteUser(const transport::User& user) = 0;

        virtual void updateUser(const transport::User& curUser, const transport::User& newUser) = 0;

        virtual std::vector<transport::User> searchUsers(const transport::User::search_t& searchUser) = 0;

    };
}

#endif //COURSEWORKDB_IUSERTRANSACTIONFACTORY_H
