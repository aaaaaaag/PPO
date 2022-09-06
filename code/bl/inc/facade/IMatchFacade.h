//
// Created by denis on 03.08.2022.
//

#ifndef COURSEWORKDB_IMATCHFACADE_H
#define COURSEWORKDB_IMATCHFACADE_H

#include "Match.h"
#include <vector>
#include "IFacadeErrorProcessor.h"

namespace polytour::bl::facade {
    class IMatchFacade: public IFacadeErrorProcessor{
    public:

        ~IMatchFacade() override = default;

        virtual void create(const transport::Match& match) = 0;

        virtual void erase(const transport::Match& match) = 0;

        virtual void update(const transport::Match& curMatch, const transport::Match& newMatch) = 0;

        virtual std::vector<transport::Match> getMatches(const transport::Match::search_t& search) = 0;

        virtual void finish(const polytour::transport::Match &match, const transport::User& winner) = 0;
    };
}

#endif //COURSEWORKDB_IMATCHFACADE_H
