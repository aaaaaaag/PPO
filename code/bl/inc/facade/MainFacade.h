//
// Created by denis on 04.08.2022.
//

#ifndef COURSEWORKDB_MAINFACADE_H
#define COURSEWORKDB_MAINFACADE_H

#include "IMainFacade.h"
#include "repositories/roles/IRole.h"

namespace polytour::bl::facade {
    class MainFacade: public IMainFacade{
    public:

        MainFacade(std::shared_ptr<IUserFacade> userFacade,
                   std::shared_ptr<ITournamentFacade> tournamentFacade,
                   std::shared_ptr<IMatchFacade> matchFacade);

        std::shared_ptr<IUserFacade> userAPI() override;

        std::shared_ptr<ITournamentFacade> tournamentAPI() override;

        std::shared_ptr<IMatchFacade> matchAPI() override;

    private:

        std::shared_ptr<IUserFacade> _pUserFacade;
        std::shared_ptr<ITournamentFacade> _pTournamentFacade;
        std::shared_ptr<IMatchFacade> _pMatchFacade;
    };
}


#endif //COURSEWORKDB_MAINFACADE_H
