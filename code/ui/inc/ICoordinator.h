//
// Created by denis on 03.08.2022.
//

#ifndef COURSEWORKDB_ICOORDINATOR_H
#define COURSEWORKDB_ICOORDINATOR_H

#include <string>
#include "facade/IMainFacade.h"
#include "ErrorObj.h"

namespace polytour::ui {
    class ICoordinator {
    public:

        virtual ~ICoordinator() = default;

        virtual std::optional<bl::ErrorObj> authorize(const std::string& nick, const std::string& pass) = 0;

        virtual polytour::bl::facade::IMainFacade& getMainAPI() = 0;

        virtual std::optional<bl::ErrorObj> signUp(transport::User user) = 0;

        virtual std::optional<bl::ErrorObj> toSignIn() = 0;

        virtual std::optional<bl::ErrorObj> toSignUp() = 0;

        virtual std::optional<bl::ErrorObj> toMainMenu() = 0;

        virtual std::optional<bl::ErrorObj> toUpdateUser() = 0;

        virtual std::optional<bl::ErrorObj> toTournamentCreation() = 0;

        virtual std::optional<bl::ErrorObj> toTournament(const transport::Tournament& tournament) = 0;

    };
}

#endif //COURSEWORKDB_ICOORDINATOR_H
