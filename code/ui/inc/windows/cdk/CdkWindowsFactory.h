//
// Created by denis on 05.08.2022.
//

#ifndef COURSEWORKDB_CDKWINDOWSFACTORY_H
#define COURSEWORKDB_CDKWINDOWSFACTORY_H

#include "windows/IWindowsFactory.h"
#include "ICoordinator.h"

namespace polytour::ui {
    class CdkWindowsFactory: public IWindowsFactory{
    public:

        explicit CdkWindowsFactory(const std::shared_ptr<ICoordinator>& coordinator);

        std::unique_ptr<IWindow> createAuthorizationWindow() override;

        std::unique_ptr<IWindow> createMainMenuWindow() override;

        std::unique_ptr<IWindow> createTournamentWindow(const transport::Tournament& tournament) override;

        std::unique_ptr<IWindow> createUserInfoWindow() override;

        std::unique_ptr<IWindow> createSignUpWindow() override;

        std::unique_ptr<IWindow> createTournamentCreationWindow() override;

    private:

        std::weak_ptr<ICoordinator> _pCoordinator;

    };
}


#endif //COURSEWORKDB_CDKWINDOWSFACTORY_H
