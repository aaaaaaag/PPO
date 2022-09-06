//
// Created by denis on 7/23/22.
//

#ifndef COURSEWORKDB_USERTRANSACTIONFACTORY_H
#define COURSEWORKDB_USERTRANSACTIONFACTORY_H

#include "IUserTransactionFactory.h"
#include "repositories/IRepositoryFactoryCreator.h"
#include <memory>

namespace polytour::bl::transaction {
    class UserTransactionFactory: public IUserTransactionFactory {
    public:

        explicit UserTransactionFactory(std::shared_ptr<db::repository::IRepositoryFactoryCreator> factoryCreator);

        void addUser(const transport::User &user) override;

        void deleteUser(const transport::User &user) override;

        void updateUser(const transport::User &curUser, const transport::User &newUser) override;

        std::vector<transport::User> searchUsers(const transport::User::search_t &searchUser) override;

    private:

        std::shared_ptr<db::repository::IRepositoryFactoryCreator> _factoryCreator;
        transport::User _curUser;

    };
}


#endif //COURSEWORKDB_USERTRANSACTIONFACTORY_H
