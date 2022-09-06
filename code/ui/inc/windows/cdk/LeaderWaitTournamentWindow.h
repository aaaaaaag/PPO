//
// Created by denis on 19.08.2022.
//

#ifndef COURSEWORKDB_LEADERWAITTOURNAMENTWINDOW_H
#define COURSEWORKDB_LEADERWAITTOURNAMENTWINDOW_H


#include "windows/ITournamentWindow.h"
#include <memory>
#include "ICoordinator.h"

namespace polytour::ui::cdk {
    class LeaderWaitTournamentWindow: public ITournamentWindow {
    public:

        explicit LeaderWaitTournamentWindow(const std::shared_ptr<ICoordinator>& coordinator,
                                       const transport::Tournament& tournament);

        ~LeaderWaitTournamentWindow() override;

        void destroy() override;

        void init() override;

    private:

        class Impl;
        std::unique_ptr<Impl> _pImpl;
    };
}


#endif //COURSEWORKDB_LEADERWAITTOURNAMENTWINDOW_H
