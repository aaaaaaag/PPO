//
// Created by denis on 04.08.2022.
//

#ifndef COURSEWORKDB_MATCHTRANSACTIONFACTORY_H
#define COURSEWORKDB_MATCHTRANSACTIONFACTORY_H

#include "IMatchTransactionFactory.h"
#include "repositories/IRepositoryFactoryCreator.h"
#include <memory>

namespace polytour::bl::transaction {
    class MatchTransactionFactory: public IMatchTransactionFactory{
    public:

        explicit MatchTransactionFactory(std::shared_ptr<db::repository::IRepositoryFactoryCreator> factoryCreator);

        void create(const transport::Match &match) override;

        void erase(const transport::Match &match) override;

        std::vector<transport::Match> getMatches(const transport::Match::search_t &search) override;

        void updateMatch(const transport::Match &curMatch, const transport::Match &newMatch) override;

        void finish(const transport::Match &match, const transport::User& winner) override;

    private:

        std::shared_ptr<db::repository::IRepositoryFactoryCreator> _factoryCreator;
        transport::User _curUser;

    };
}


#endif //COURSEWORKDB_MATCHTRANSACTIONFACTORY_H
