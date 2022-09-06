//
// Created by denis on 21.08.2022.
//

#ifndef COURSEWORKDB_LEADERSTARTEDTOURNAMENTWINDOW_H
#define COURSEWORKDB_LEADERSTARTEDTOURNAMENTWINDOW_H

#include "windows/ITournamentWindow.h"
#include <memory>
#include "ICoordinator.h"

namespace polytour::ui::cdk {
    class LeaderStartedTournamentWindow: public ITournamentWindow {
    public:

        explicit LeaderStartedTournamentWindow(const std::shared_ptr<ICoordinator>& coordinator,
                                            const transport::Tournament& tournament);

        ~LeaderStartedTournamentWindow() override;

        void destroy() override;

        void init() override;

    private:

        class Impl;
        std::unique_ptr<Impl> _pImpl;
    };
}

#endif //COURSEWORKDB_LEADERSTARTEDTOURNAMENTWINDOW_H
