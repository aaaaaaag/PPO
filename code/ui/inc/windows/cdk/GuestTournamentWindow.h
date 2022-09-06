//
// Created by denis on 10.08.2022.
//

#ifndef COURSEWORKDB_GUESTTOURNAMENTWINDOW_H
#define COURSEWORKDB_GUESTTOURNAMENTWINDOW_H

#include "windows/ITournamentWindow.h"
#include <memory>
#include "ICoordinator.h"

namespace polytour::ui::cdk {
    class GuestTournamentWindow: public ITournamentWindow {
    public:

        explicit GuestTournamentWindow(const std::shared_ptr<ICoordinator>& coordinator,
                                       const transport::Tournament& tournament);

        ~GuestTournamentWindow() override;

        void destroy() override;

        void init() override;

    private:

        class Impl;
        std::unique_ptr<Impl> _pImpl;
    };
}

#endif //COURSEWORKDB_GUESTTOURNAMENTWINDOW_H
