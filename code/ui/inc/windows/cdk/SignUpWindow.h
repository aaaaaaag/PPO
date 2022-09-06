//
// Created by denis on 06.08.2022.
//

#ifndef COURSEWORKDB_SIGNUPWINDOW_H
#define COURSEWORKDB_SIGNUPWINDOW_H

#include "windows/ISingUpWindow.h"
#include "ICoordinator.h"
#include <memory>

namespace polytour::ui::cdk {
    class SignUpWindow: public ISingUpWindow{
    public:

        explicit SignUpWindow(const std::shared_ptr<ICoordinator>& coordinator);

        ~SignUpWindow() override;

        void destroy() override;

        void init() override;

    private:

        class Impl;
        std::unique_ptr<Impl> _pImpl;

    };
}

#endif //COURSEWORKDB_SIGNUPWINDOW_H
