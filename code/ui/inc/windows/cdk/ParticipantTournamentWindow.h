//
// Created by denis on 16.08.2022.
//

#ifndef COURSEWORKDB_PARTICIPANTTOURNAMENTWINDOW_H
#define COURSEWORKDB_PARTICIPANTTOURNAMENTWINDOW_H

#include "windows/ITournamentWindow.h"
#include <memory>
#include "ICoordinator.h"

namespace polytour::ui::cdk {
    class ParticipantTournamentWindow: public ITournamentWindow {
    public:

        explicit ParticipantTournamentWindow(const std::shared_ptr<ICoordinator>& coordinator,
                                       const transport::Tournament& tournament);

        ~ParticipantTournamentWindow() override;

        void destroy() override;

        void init() override;

    private:

        class Impl;
        std::unique_ptr<Impl> _pImpl;
    };
}

#endif //COURSEWORKDB_PARTICIPANTTOURNAMENTWINDOW_H
