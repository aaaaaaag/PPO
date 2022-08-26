//
// Created by denis on 04.08.2022.
//

#ifndef COURSEWORKDB_TOURNAMENTFACADE_H
#define COURSEWORKDB_TOURNAMENTFACADE_H

#include "ITournamentFacade.h"
#include "transactions/ITournamentTransactionFactory.h"
#include <memory>

namespace polytour::bl::facade {
    class TournamentFacade: public ITournamentFacade{
    public:

        explicit TournamentFacade(std::shared_ptr<transaction::ITournamentTransactionFactory> transactionFactory);

        void create(const transport::Tournament &tournament) override;

        void remove(const transport::Tournament &tournament) override;

        std::vector<transport::Tournament> getTournaments(const transport::Tournament::search_t &search) override;

        std::vector<transport::User> getTournamentParticipants(const transport::Tournament &tournament) override;

        void join(const transport::Tournament &tournament) override;

        void leave(const transport::Tournament &tournament) override;

        void start(const transport::Tournament &tournament) override;

    private:

        std::shared_ptr<transaction::ITournamentTransactionFactory> _pTransactionFactory;

    };
}

#endif //COURSEWORKDB_TOURNAMENTFACADE_H
