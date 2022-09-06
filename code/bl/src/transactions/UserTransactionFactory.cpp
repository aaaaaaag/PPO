//
// Created by denis on 7/23/22.
//

#include "transactions/UserTransactionFactory.h"

#include <utility>
#include "repositories/RepositoryFactory.h"
#include "AuthUserSingleton.h"
#include "CurrentRoleSingleton.h"

void polytour::bl::transaction::UserTransactionFactory::addUser(const polytour::transport::User &user) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    repoFactory->getUserRepository()->addObj(user);
}

void polytour::bl::transaction::UserTransactionFactory::deleteUser(const polytour::transport::User &user) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);

    auto* repoUser = repoFactory->getUserRepository();
    auto* repoTournament = repoFactory->getTournamentRepository();
    auto* repoMatch = repoFactory->getMatchRepository();
    auto* repoTourParticipants = repoFactory->getTournamentParticipantsRepository();

    // Process tournaments that user has organized. Delete them all
    auto userOrganizedTournaments = repoTournament->findObj({.organizer_id_ = user.id});
    for (const auto& tournament: userOrganizedTournaments) {
        auto tournamentMatches = repoMatch->findObj({.tournament_id_ = tournament.id});
        for (const auto& match: tournamentMatches)
            repoMatch->deleteObj(match);
        repoTournament->deleteObj(tournament);
    }

    // Process tournaments where user take part. Remove user from list of partitions and auto lose current match
    auto userParticipateTournaments = repoTourParticipants->findObj({.participant_id_ = user.id});
    for (const auto& tournament: userParticipateTournaments) {
        auto tournamentMatches = repoMatch->findObj({.tournament_id_ = tournament.id});
        for (const auto& match: tournamentMatches) {
            if (match.status == match.status_wait_participants()) {
                if (match.participant_1_id.hasValue() && match.participant_1_id.getValue() == user.id) {
                   auto updatedMatch = match;
                   updatedMatch.status = updatedMatch.status_finished();
                   //updatedMatch.winner_id =
                }
            }
        }
    }

    repoUser->deleteObj(user);
}

void polytour::bl::transaction::UserTransactionFactory::updateUser(const polytour::transport::User &curUser,
                                                                   const polytour::transport::User &newUser) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    repoFactory->getUserRepository()->updateObj(curUser, newUser);
}

std::vector<polytour::transport::User>
polytour::bl::transaction::UserTransactionFactory::searchUsers(const polytour::transport::User::search_t &searchUser) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    return repoFactory->getUserRepository()->findObj(searchUser);
}

polytour::bl::transaction::UserTransactionFactory::UserTransactionFactory(
        std::shared_ptr<db::repository::IRepositoryFactoryCreator> factoryCreator):
_factoryCreator(std::move(factoryCreator)),
_curUser(*AuthUserSingleton::getInstance()){}

