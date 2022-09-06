//
// Created by denis on 02.08.2022.
//

#ifndef COURSEWORKDB_AUTHORIZATIONWINDOW_H
#define COURSEWORKDB_AUTHORIZATIONWINDOW_H

#include "windows/IAuthorizationWindow.h"
#include "ICoordinator.h"
#include <memory>

namespace polytour::ui::cdk {
    class AuthorizationWindow : public IAuthorizationWindow {
    public:

        explicit AuthorizationWindow(const std::shared_ptr<ICoordinator>& coordinator);

        ~AuthorizationWindow() override;

        void destroy() override;

        void init() override;

    private:

        class Impl;
        std::unique_ptr<Impl> _pImpl;

    };
}


#endif //COURSEWORKDB_AUTHORIZATIONWINDOW_H
