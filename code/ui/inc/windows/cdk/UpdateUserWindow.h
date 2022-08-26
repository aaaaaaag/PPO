//
// Created by denis on 08.08.2022.
//

#ifndef COURSEWORKDB_UPDATEUSERWINDOW_H
#define COURSEWORKDB_UPDATEUSERWINDOW_H

#include "windows/IUpdateUserWindow.h"
#include <memory>
#include "ICoordinator.h"

namespace polytour::ui::cdk {
    class UpdateUserWindow: public IUpdateUserWindow {
    public:

        UpdateUserWindow(const std::shared_ptr<ICoordinator>& coordinator);

        ~UpdateUserWindow() override;

        void destroy() override;

        void init() override;


    private:

        class Impl;
        std::unique_ptr<Impl> _pImpl;

    };
}


#endif //COURSEWORKDB_UPDATEUSERWINDOW_H
