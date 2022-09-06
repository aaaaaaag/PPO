//
// Created by denis on 04.08.2022.
//

#ifndef COURSEWORKDB_USERFACADE_H
#define COURSEWORKDB_USERFACADE_H

#include <memory>
#include "IUserFacade.h"
#include "transactions/IUserTransactionFactory.h"

namespace polytour::bl::facade {
    class UserFacade: public IUserFacade {
    public:

        explicit UserFacade(std::shared_ptr<transaction::IUserTransactionFactory> transactionFactory);

        transport::User currentUser() override;

        void auth(const std::string &nick, const std::string &pass) override;

        std::vector<transport::User> getUsers(transport::User::search_t search) override;

        void deleteUser(const transport::User &user) override;

        void regNewUser(const transport::User &user) override;

        void updateUser(const transport::User &user) override;

    private:

        std::shared_ptr<transaction::IUserTransactionFactory> _pTransactionFactory;

    };
}


#endif //COURSEWORKDB_USERFACADE_H
