//
// Created by denis on 7/23/22.
//

#ifndef COURSEWORKDB_TOURNAMENTTRANSACTIONFACTORY_H
#define COURSEWORKDB_TOURNAMENTTRANSACTIONFACTORY_H

#include "ITournamentTransactionFactory.h"
#include "repositories/IRepositoryFactoryCreator.h"
#include <memory>

namespace polytour::bl::transaction {
    class TournamentTransactionFactory: public ITournamentTransactionFactory {
    public:

        explicit TournamentTransactionFactory(std::shared_ptr<db::repository::IRepositoryFactoryCreator> factoryCreator);

        void create(const transport::Tournament &tournament) override;

        void erase(const transport::Tournament &tournament) override;

        void join(const transport::Tournament &tournament) override;

        void leave(const transport::Tournament &tournament) override;

        void start(const transport::Tournament &tournament) override;

        std::vector<transport::Tournament> search(const transport::Tournament::search_t &search) override;

        std::vector<transport::User> getParticipants(const transport::Tournament &tournament) override;

    private:

        std::shared_ptr<db::repository::IRepositoryFactoryCreator> _factoryCreator;
        transport::User* _curUser;

    };
}


#endif //COURSEWORKDB_TOURNAMENTTRANSACTIONFACTORY_H
