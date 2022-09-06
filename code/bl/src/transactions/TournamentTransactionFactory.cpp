//
// Created by denis on 7/23/22.
//

#include "transactions/TournamentTransactionFactory.h"
#include <utility>
#include <cmath>
#include "repositories/RepositoryFactory.h"
#include "AuthUserSingleton.h"
#include "NotCriticalError.h"
#include "CurrentRoleSingleton.h"


void finish(const std::shared_ptr<polytour::db::repository::IRepositoryFactory>& repoFactory,
            const polytour::transport::Match &match, const polytour::transport::User& winner) {

    auto matchRepo = repoFactory->getMatchRepository();
    auto updatedMatch = match;
    updatedMatch.status = polytour::transport::Match::status_finished();
    if (updatedMatch.participant_1_id.getValue() == winner.id) {
        updatedMatch.winner_id = winner.id;
        updatedMatch.loser_id = updatedMatch.participant_2_id;
    } else if (updatedMatch.participant_2_id.getValue() == winner.id) {
        updatedMatch.winner_id = winner.id;
        updatedMatch.loser_id = updatedMatch.participant_1_id;
    } else {
        throw polytour::CriticalError("Match take invalid user for finish operation");
    }
    matchRepo->updateObj(match, updatedMatch);

    // Search for matches in the next tour
    auto matchTour = match.tour;
    auto nextTourMatches = matchRepo->findObj({
                                                      .tournament_id_ = match.tournament_id,
                                                      .tour_ = matchTour + 1});
    if (!nextTourMatches.empty()) {
        for(const auto& nextMatch: nextTourMatches) {
            auto updatedNextMatch = nextMatch;
            if (nextMatch.participant_1_id.isNull()) {
                updatedNextMatch.participant_1_id = winner.id;
            } else if (nextMatch.participant_2_id.isNull()) {
                updatedNextMatch.participant_2_id = winner.id;
                updatedNextMatch.status = polytour::transport::Match::status_wait_tribes();
            } else {
                continue;
            }
            matchRepo->updateObj(nextMatch, updatedNextMatch);
            break;
        }
    } else {
        auto tournamentRepo = repoFactory->getTournamentRepository();
        auto tournament = tournamentRepo->findObj({.id_ = match.tournament_id})[0];
        auto updatedTournament = tournament;
        updatedTournament.status = polytour::transport::Tournament::status_finished();
        tournamentRepo->updateObj(tournament, updatedTournament);
    }
}

void
polytour::bl::transaction::TournamentTransactionFactory::create(const polytour::transport::Tournament &tournament) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    repoFactory->getTournamentRepository()->addObj(tournament);
}

void polytour::bl::transaction::TournamentTransactionFactory::erase(const polytour::transport::Tournament &tournament) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    auto* tournamentRepo = repoFactory->getTournamentRepository();
    auto* matchRepo = repoFactory->getMatchRepository();
    auto* tournamentParticipantsRepo = repoFactory->getTournamentParticipantsRepository();

    auto tournamentMatches = matchRepo->findObj({.tournament_id_ = tournament.id});
    for (const auto& match: tournamentMatches)
        matchRepo->deleteObj(match);

    auto tournamentParticipants = tournamentParticipantsRepo->findObj({.tournament_id_ = tournament.id});
    for (const auto& participant: tournamentParticipants)
        tournamentParticipantsRepo->deleteObj(participant);

    tournamentRepo->deleteObj(tournament);
}

void polytour::bl::transaction::TournamentTransactionFactory::join(const polytour::transport::Tournament &tournament) {
    if ((tournament.cur_participants_num == tournament.max_participants_num) and
        (tournament.status != tournament.status_wait_for_participants()))
        return;

    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    auto* tournamentRepo = repoFactory->getTournamentRepository();
    auto* tournamentParticipantsRepo = repoFactory->getTournamentParticipantsRepository();

    auto updatedTournament = tournament;
    updatedTournament.cur_participants_num += 1;
    tournamentRepo->updateObj(tournament, updatedTournament);

    transport::TournamentParticipant tournamentParticipant;
    tournamentParticipant.tournament_id = tournament.id;
    tournamentParticipant.participant_id = AuthUserSingleton::getInstance()->id;
    tournamentParticipantsRepo->addObj(tournamentParticipant);
}

void polytour::bl::transaction::TournamentTransactionFactory::leave(const polytour::transport::Tournament &tournament) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    auto* tournamentRepo = repoFactory->getTournamentRepository();
    auto* tournamentParticipantsRepo = repoFactory->getTournamentParticipantsRepository();
    auto* matchRepo = repoFactory->getMatchRepository();
    auto* userRepo = repoFactory->getUserRepository();

    auto updatedTournament = tournament;
    updatedTournament.cur_participants_num -= 1;
    tournamentRepo->updateObj(tournament, updatedTournament);

    auto participant = tournamentParticipantsRepo->findObj({
        .tournament_id_ = tournament.id,
        .participant_id_ = _curUser->id
    });
    if (participant.size() != 1)
        throw polytour::NotCriticalError("User doesn't take part in passed tournament");

    tournamentParticipantsRepo->deleteObj(participant[0]);

    transport::Match::search_t matchSearchObj1;
    matchSearchObj1.participant_1_id_ = _curUser->id;
    matchSearchObj1.tournament_id_ = tournament.id;
    auto matchesWhereUserParticipant1 = matchRepo->findObj(matchSearchObj1);

    for (const auto& match: matchesWhereUserParticipant1) {
        auto updatedMatch = match;
        if (match.status == polytour::transport::Match::status_started() ||
            match.status == polytour::transport::Match::status_wait_tribes()) {
            auto participant2 = userRepo->findObj({.id_ = match.participant_2_id.getValue()})[0];
            finish(repoFactory, match, participant2);
        }
        else if (match.status == polytour::transport::Match::status_wait_participants()) {
            updatedMatch.loser_id = updatedMatch.participant_1_id;
        }
        matchRepo->updateObj(match, updatedMatch);
    }

    transport::Match::search_t matchSearchObj2;
    matchSearchObj2.participant_2_id_ = _curUser->id;
    auto matchesWhereUserParticipant2 = matchRepo->findObj(matchSearchObj2);

    for (const auto& match: matchesWhereUserParticipant2) {
        auto updatedMatch = match;
        if (match.status == polytour::transport::Match::status_started() ||
            match.status == polytour::transport::Match::status_wait_tribes()) {
            auto participant1 = userRepo->findObj({.id_ = match.participant_1_id.getValue()})[0];
            finish(repoFactory, match, participant1);
        }
        else if (match.status == polytour::transport::Match::status_wait_participants()) {
            updatedMatch.loser_id = updatedMatch.participant_2_id;
        }
        matchRepo->updateObj(match, updatedMatch);
    }
}

std::vector<polytour::transport::Tournament> polytour::bl::transaction::TournamentTransactionFactory::search(
        const polytour::transport::Tournament::search_t &search) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    return repoFactory->getTournamentRepository()->findObj(search);
}

std::vector<polytour::transport::User> polytour::bl::transaction::TournamentTransactionFactory::getParticipants(
        const polytour::transport::Tournament &tournament) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    auto participants = repoFactory->getTournamentParticipantsRepository()->findObj(
            {.tournament_id_ = tournament.id});

    std::vector<polytour::transport::User> result;
    for (const auto& participant: participants) {
        result.emplace_back(repoFactory->getUserRepository()->findObj({.id_ = participant.participant_id})[0]);
    }
    return result;
}

void polytour::bl::transaction::TournamentTransactionFactory::start(const polytour::transport::Tournament &tournament) {
    auto repoFactory = _factoryCreator->create(CurrentRoleSingleton::getInstance()->role);
    auto tournamentRepo = repoFactory->getTournamentRepository();
    auto matchRepo = repoFactory->getMatchRepository();

    auto updatedTournament = tournament;
    updatedTournament.status = transport::Tournament::status_started();
    tournamentRepo->updateObj(tournament, updatedTournament);

    auto maxParticipants = tournament.max_participants_num;
    auto participants = getParticipants(tournament);
    auto tours = log2(maxParticipants);

    transport::Match matchTemplate;
    matchTemplate.status = transport::Match::status_wait_participants();
    matchTemplate.tournament_id = tournament.id;

    // Init first tour
    for (int i = 0; i < pow(2, tours - 1); i++) {
        auto curMatch = matchTemplate;
        curMatch.tour = 1;
        curMatch.status = transport::Match::status_wait_tribes();
        curMatch.participant_1_id = participants[i * 2].id;
        curMatch.participant_2_id = participants[i * 2 + 1].id;
        matchRepo->addObj(curMatch);
    }

    // Init other tours
    for (int i = 2; i <= tours; i++) {
        for (int j = 0; j < pow(2, tours - i); j++) {
            auto curMatch = matchTemplate;
            curMatch.tour = i;
            matchRepo->addObj(curMatch);
        }
    }

}

polytour::bl::transaction::TournamentTransactionFactory::TournamentTransactionFactory(
        std::shared_ptr<db::repository::IRepositoryFactoryCreator> factoryCreator):
        _factoryCreator(std::move(factoryCreator)),
        _curUser(AuthUserSingleton::getInstance()) {}