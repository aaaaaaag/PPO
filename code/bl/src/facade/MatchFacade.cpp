//
// Created by denis on 04.08.2022.
//

#include "facade/MatchFacade.h"

#include <utility>
#include "transactions/MatchTransactionFactory.h"
#include "AuthUserSingleton.h"
#include "repositories/roles/GuestRole.h"
#include "Logger.h"

void polytour::bl::facade::MatchFacade::create(const polytour::transport::Match &match) {
    transport::Logger::info("Called create new match func");
    processError([this, match](){
        _pTransactionFactory->create(match);
    });
}

void polytour::bl::facade::MatchFacade::erase(const polytour::transport::Match &match) {
    transport::Logger::info("Called erase match func");
    processError([this, match](){
        _pTransactionFactory->erase(match);
    });
}

std::vector<polytour::transport::Match>
polytour::bl::facade::MatchFacade::getMatches(const polytour::transport::Match::search_t &search) {
    transport::Logger::info("Called get matches func");
    std::vector<polytour::transport::Match> result;
    processError([this, search, &result]() {
        result = _pTransactionFactory->getMatches(search);
    });
    return result;
}

void polytour::bl::facade::MatchFacade::update(const polytour::transport::Match &curMatch,
                                               const polytour::transport::Match &newMatch) {
    transport::Logger::info("Called update match func");
    processError([this, curMatch, newMatch]() {
        _pTransactionFactory->updateMatch(curMatch, newMatch);
    });
}

void polytour::bl::facade::MatchFacade::finish(const polytour::transport::Match &match,
                                               const polytour::transport::User &winner) {
    transport::Logger::info("Called finish match func");
    processError([this, match, winner]() {
        _pTransactionFactory->finish(match, winner);
    });
}

polytour::bl::facade::MatchFacade::MatchFacade(std::shared_ptr<transaction::IMatchTransactionFactory> transactionFactory):
_pTransactionFactory(std::move(transactionFactory)){}
