//
// Created by denis on 29.07.2022.
//

#ifndef COURSEWORKDB_IMATCHTRANSACTIONFACTORY_H
#define COURSEWORKDB_IMATCHTRANSACTIONFACTORY_H

#include "User.h"
#include "Match.h"
#include <vector>

namespace polytour::bl::transaction {
    class IMatchTransactionFactory {
    public:

        virtual ~IMatchTransactionFactory() = default;

        virtual void create(const transport::Match& match) = 0;

        virtual void erase(const transport::Match& match) = 0;

        virtual std::vector<transport::Match> getMatches(const transport::Match::search_t& search) = 0;

        virtual void updateMatch(const transport::Match& curMatch, const transport::Match& newMatch) = 0;

        virtual void finish(const transport::Match& match, const transport::User& winner) = 0;

    };
}

#endif //COURSEWORKDB_IMATCHTRANSACTIONFACTORY_H
