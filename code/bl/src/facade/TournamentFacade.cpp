//
// Created by denis on 04.08.2022.
//

#include "facade/TournamentFacade.h"

#include <utility>
#include "transactions/TournamentTransactionFactory.h"
#include "AuthUserSingleton.h"
#include "repositories/roles/GuestRole.h"
#include "NotCriticalError.h"

void polytour::bl::facade::TournamentFacade::create(const polytour::transport::Tournament &tournament) {
    transport::Logger::info("Called create new tournament func");
    processError([this, tournament](){
        _pTransactionFactory->create(tournament);
    });
}

void polytour::bl::facade::TournamentFacade::remove(const polytour::transport::Tournament &tournament) {
    transport::Logger::info("Called erase tournament func");
    processError([this, tournament](){
        if (tournament.name.empty())
            throw polytour::NotCriticalError("name field doesn't passed");
        if (tournament.status.empty())
            throw polytour::CriticalError("status doesn't passed");
        if (tournament.organizer_id == -1)
            throw polytour::CriticalError("organizer id doesn't passed");
        if (tournament.max_participants_num == -1)
            throw polytour::NotCriticalError("participants count field doesn't passed");
        _pTransactionFactory->erase(tournament);
    });
}

std::vector<polytour::transport::Tournament>
polytour::bl::facade::TournamentFacade::getTournaments(const polytour::transport::Tournament::search_t &search) {
    transport::Logger::info("Called get tournaments func");
    std::vector<polytour::transport::Tournament> result;
    processError([this, search, &result]() {
        result = _pTransactionFactory->search(search);
    });
    return result;
}

std::vector<polytour::transport::User>
polytour::bl::facade::TournamentFacade::getTournamentParticipants(const polytour::transport::Tournament &tournament) {
    transport::Logger::info("Called get tournament participants func");
    std::vector<polytour::transport::User> result;
    processError([this, tournament, &result]() {
        result = _pTransactionFactory->getParticipants(tournament);
    });
    return result;
}

void polytour::bl::facade::TournamentFacade::join(const polytour::transport::Tournament &tournament) {
    transport::Logger::info("Called join tournament func");
    processError([this, tournament](){
        _pTransactionFactory->join(tournament);
    });
}

void polytour::bl::facade::TournamentFacade::leave(const polytour::transport::Tournament &tournament) {
    transport::Logger::info("Called leave tournament func");
    processError([this, tournament](){
        _pTransactionFactory->leave(tournament);
    });
}

void polytour::bl::facade::TournamentFacade::start(const polytour::transport::Tournament &tournament) {
    transport::Logger::info("Called start tournament func");
    processError([this, tournament](){
        _pTransactionFactory->start(tournament);
    });
}

polytour::bl::facade::TournamentFacade::TournamentFacade(
        std::shared_ptr<transaction::ITournamentTransactionFactory> transactionFactory):
_pTransactionFactory(std::move(transactionFactory)){}
