//
// Created by denis on 04.08.2022.
//

#ifndef COURSEWORKDB_MAINMENUWINDOW_H
#define COURSEWORKDB_MAINMENUWINDOW_H

#include "windows/IMainMenuWindow.h"
#include "ICoordinator.h"
#include <memory>

namespace polytour::ui {
    class MainMenuWindow: public IMainMenuWindow {
    public:

        explicit MainMenuWindow(const std::shared_ptr<ICoordinator>& coordinator);

        ~MainMenuWindow() override;

        void destroy() override;

        void init() override;

    private:

        class Impl;
        std::unique_ptr<Impl> _pImpl;
    };
}

#endif //COURSEWORKDB_MAINMENUWINDOW_H
