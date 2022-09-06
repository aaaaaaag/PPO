//
// Created by denis on 09.08.2022.
//

#ifndef COURSEWORKDB_TOURNAMENTCREATIONWINDOW_H
#define COURSEWORKDB_TOURNAMENTCREATIONWINDOW_H

#include "windows/ITournamentCreationWindow.h"
#include "ICoordinator.h"
#include <memory>

namespace polytour::ui::cdk {
    class TournamentCreationWindow: public ITournamentCreationWindow{
    public:

        explicit TournamentCreationWindow(const std::shared_ptr<ICoordinator>& coordinator);

        ~TournamentCreationWindow() override;

        void destroy() override;

        void init() override;

    private:

        class Impl;
        std::unique_ptr<Impl> _pImpl;

    };
}


#endif //COURSEWORKDB_TOURNAMENTCREATIONWINDOW_H
