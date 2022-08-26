//
// Created by denis on 7/23/22.
//

#ifndef COURSEWORKDB_ITOURNAMENTTRANSACTIONFACTORY_H
#define COURSEWORKDB_ITOURNAMENTTRANSACTIONFACTORY_H

#include "User.h"
#include "Tournament.h"
#include "TournamentParticipant.h"
#include <vector>

namespace polytour::bl::transaction {
    class ITournamentTransactionFactory {
    public:

        virtual ~ITournamentTransactionFactory() = default;

        virtual void create(const transport::Tournament& tournament) = 0;

        virtual void erase(const transport::Tournament& tournament) = 0;

        virtual void join(const transport::Tournament& tournament) = 0;

        virtual void leave(const transport::Tournament& tournament) = 0;

        virtual void start(const transport::Tournament& tournament) = 0;

        virtual std::vector<transport::Tournament> search(const transport::Tournament::search_t& search) = 0;

        virtual std::vector<transport::User> getParticipants(const transport::Tournament& tournament) = 0;
    };
}

#endif //COURSEWORKDB_ITOURNAMENTTRANSACTIONFACTORY_H
