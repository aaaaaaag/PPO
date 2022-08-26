//
// Created by denis on 03.08.2022.
//

#ifndef COURSEWORKDB_ITOURNAMENTFACADE_H
#define COURSEWORKDB_ITOURNAMENTFACADE_H

#include "Tournament.h"
#include <vector>
#include "IFacadeErrorProcessor.h"

namespace polytour::bl::facade {
    class ITournamentFacade: public IFacadeErrorProcessor{
    public:

        ~ITournamentFacade() override = default;

        virtual void create(const transport::Tournament& tournament) = 0;

        virtual void remove(const transport::Tournament& tournament) = 0;

        virtual std::vector<transport::Tournament> getTournaments(const transport::Tournament::search_t& search) = 0;

        virtual std::vector<transport::User> getTournamentParticipants(const transport::Tournament& tournament) = 0;

        virtual void join(const transport::Tournament& tournament) = 0;

        virtual void leave(const transport::Tournament& tournament) = 0;

        virtual void start(const transport::Tournament &tournament) = 0;
    };
}

#endif //COURSEWORKDB_ITOURNAMENTFACADE_H
