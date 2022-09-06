//
// Created by denis on 04.08.2022.
//

#include "facade/MainFacade.h"

#include <utility>
#include "facade/TournamentFacade.h"
#include "facade/UserFacade.h"
#include "facade/MatchFacade.h"

std::shared_ptr<polytour::bl::facade::IUserFacade> polytour::bl::facade::MainFacade::userAPI() {
    return _pUserFacade;
}

std::shared_ptr<polytour::bl::facade::ITournamentFacade> polytour::bl::facade::MainFacade::tournamentAPI() {
    return _pTournamentFacade;
}

std::shared_ptr<polytour::bl::facade::IMatchFacade> polytour::bl::facade::MainFacade::matchAPI() {
    return _pMatchFacade;
}

polytour::bl::facade::MainFacade::MainFacade(std::shared_ptr<IUserFacade> userFacade,
                                             std::shared_ptr<ITournamentFacade> tournamentFacade,
                                             std::shared_ptr<IMatchFacade> matchFacade):
                                             _pUserFacade(std::move(userFacade)),
                                             _pTournamentFacade(std::move(tournamentFacade)),
                                             _pMatchFacade(std::move(matchFacade)){

}
