//
// Created by denis on 03.08.2022.
//

#ifndef COURSEWORKDB_IMAINFACADE_H
#define COURSEWORKDB_IMAINFACADE_H

#include "IMatchFacade.h"
#include "IUserFacade.h"
#include "ITournamentFacade.h"
#include <memory>

namespace polytour::bl::facade {
    class IMainFacade {
    public:

      virtual ~IMainFacade() = default;

      virtual std::shared_ptr<IUserFacade> userAPI() = 0;

      virtual std::shared_ptr<ITournamentFacade> tournamentAPI() = 0;

      virtual std::shared_ptr<IMatchFacade> matchAPI() = 0;

    };
}

#endif //COURSEWORKDB_IMAINFACADE_H
