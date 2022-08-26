//
// Created by denis on 26.08.2022.
//

#include "gtest/gtest.h"
#include "repositories/RepositoryFactoryCreator.h"

TEST(db_integration_tests, testGuestRole) {
    auto repoFactoryCreator = std::make_shared<polytour::db::repository::RepositoryFactoryCreator>();
    auto repoFactory = repoFactoryCreator->create(polytour::transport::Roles::Guest);
    auto userRepo = repoFactory->getUserRepository();
    auto tournamentRepo = repoFactory->getTournamentRepository();
    auto tournamentParticipantsRepo = repoFactory->getTournamentParticipantsRepository();
    auto matchRepo = repoFactory->getMatchRepository();

    polytour::transport::User user;
    user.id = 1;
    user.email = "email";
    user.name = "name";
    user.password = "pass";
    user.nickname = "nick";
    EXPECT_NO_THROW(userRepo->addObj(user));

    EXPECT_NO_THROW(userRepo->findObj({.id_ = user.id}));
    auto newUser = user;
    newUser.nickname = "nick2";
    EXPECT_THROW(userRepo->updateObj(user, newUser), polytour::CriticalError);
    EXPECT_THROW(userRepo->deleteObj(user), polytour::CriticalError);

    polytour::transport::Tournament tournament;
    tournament.id = 777;
    tournament.name = "name";
    tournament.max_participants_num = 4;
    tournament.cur_participants_num = 1;
    tournament.status = polytour::transport::Tournament::status_wait_for_participants();
    tournament.organizer_id = 1;
    EXPECT_THROW(tournamentRepo->addObj(tournament), polytour::CriticalError);
    EXPECT_THROW(tournamentRepo->updateObj(tournament, tournament), polytour::CriticalError);
    EXPECT_THROW(tournamentRepo->findObj({}), polytour::CriticalError);
    EXPECT_THROW(tournamentRepo->deleteObj(tournament), polytour::CriticalError);

    polytour::transport::Match match;
    match.id = 777;
    match.status = polytour::transport::Match::status_wait_participants();
    match.tournament_id = 777;
    match.tour = 1;
    EXPECT_THROW(matchRepo->addObj(match), polytour::CriticalError);
    EXPECT_THROW(matchRepo->updateObj(match, match), polytour::CriticalError);
    EXPECT_THROW(matchRepo->findObj({}), polytour::CriticalError);
    EXPECT_THROW(matchRepo->deleteObj(match), polytour::CriticalError);

    polytour::transport::TournamentParticipant participant;
    participant.id = 1;
    participant.tournament_id = 1;
    participant.participant_id = 1;
    EXPECT_THROW(tournamentParticipantsRepo->addObj(participant), polytour::CriticalError);
    EXPECT_THROW(tournamentParticipantsRepo->updateObj(participant, participant), polytour::CriticalError);
    EXPECT_THROW(tournamentParticipantsRepo->findObj({}), polytour::CriticalError);
    EXPECT_THROW(tournamentParticipantsRepo->deleteObj(participant), polytour::CriticalError);
}


TEST(db_integration_tests, testCommonUserRole) {
    auto repoFactoryCreator = std::make_shared<polytour::db::repository::RepositoryFactoryCreator>();
    auto repoFactory = repoFactoryCreator->create(polytour::transport::Roles::CommonUser);
    auto userRepo = repoFactory->getUserRepository();
    auto tournamentRepo = repoFactory->getTournamentRepository();
    auto tournamentParticipantsRepo = repoFactory->getTournamentParticipantsRepository();
    auto matchRepo = repoFactory->getMatchRepository();

    polytour::transport::User user;
    user.id = 1;
    user.email = "email";
    user.name = "name";
    user.password = "pass";
    user.nickname = "nick";
    EXPECT_THROW(userRepo->addObj(user), polytour::CriticalError);

    EXPECT_NO_THROW(userRepo->findObj({}));
    auto newUser = user;
    newUser.nickname = "nick2";
    EXPECT_NO_THROW(userRepo->updateObj(user, newUser));
    EXPECT_NO_THROW(userRepo->deleteObj(newUser));

    polytour::transport::Tournament tournament;
    tournament.id = 777;
    tournament.name = "name";
    tournament.max_participants_num = 4;
    tournament.cur_participants_num = 1;
    tournament.status = polytour::transport::Tournament::status_wait_for_participants();
    tournament.organizer_id = 1;
    EXPECT_NO_THROW(tournamentRepo->addObj(tournament));
    EXPECT_NO_THROW(tournamentRepo->updateObj(tournament, tournament));
    EXPECT_NO_THROW(tournamentRepo->findObj({}));
    EXPECT_THROW(tournamentRepo->deleteObj(tournament), polytour::CriticalError);

    polytour::transport::Match match;
    match.id = 777;
    match.status = polytour::transport::Match::status_wait_participants();
    match.tournament_id = 777;
    match.tour = 1;
    EXPECT_THROW(matchRepo->addObj(match), polytour::CriticalError);
    EXPECT_THROW(matchRepo->updateObj(match, match), polytour::CriticalError);
    EXPECT_NO_THROW(matchRepo->findObj({}));
    EXPECT_THROW(matchRepo->deleteObj(match), polytour::CriticalError);

    polytour::transport::TournamentParticipant participant;
    participant.id = 1;
    participant.tournament_id = 1;
    participant.participant_id = 1;
    EXPECT_NO_THROW(tournamentParticipantsRepo->addObj(participant));
    EXPECT_THROW(tournamentParticipantsRepo->updateObj(participant, participant), polytour::CriticalError);
    EXPECT_NO_THROW(tournamentParticipantsRepo->findObj({}));
    EXPECT_THROW(tournamentParticipantsRepo->deleteObj(participant), polytour::CriticalError);
}

TEST(db_integration_tests, testParticipantRole) {
    auto repoFactoryCreator = std::make_shared<polytour::db::repository::RepositoryFactoryCreator>();
    auto repoFactory = repoFactoryCreator->create(polytour::transport::Roles::Participant);
    auto userRepo = repoFactory->getUserRepository();
    auto tournamentRepo = repoFactory->getTournamentRepository();
    auto tournamentParticipantsRepo = repoFactory->getTournamentParticipantsRepository();
    auto matchRepo = repoFactory->getMatchRepository();

    polytour::transport::User user;
    user.id = 1;
    user.email = "email";
    user.name = "name";
    user.password = "pass";
    user.nickname = "nick";
    EXPECT_THROW(userRepo->addObj(user), polytour::CriticalError);

    EXPECT_NO_THROW(userRepo->findObj({}));
    auto newUser = user;
    newUser.nickname = "nick2";
    EXPECT_NO_THROW(userRepo->updateObj(user, newUser));
    EXPECT_NO_THROW(userRepo->deleteObj(newUser));

    polytour::transport::Tournament tournament;
    tournament.id = 777;
    tournament.name = "name";
    tournament.max_participants_num = 4;
    tournament.cur_participants_num = 1;
    tournament.status = polytour::transport::Tournament::status_wait_for_participants();
    tournament.organizer_id = 1;
    EXPECT_THROW(tournamentRepo->addObj(tournament), polytour::CriticalError);
    EXPECT_NO_THROW(tournamentRepo->updateObj(tournament, tournament));
    EXPECT_NO_THROW(tournamentRepo->findObj({}));
    EXPECT_THROW(tournamentRepo->deleteObj(tournament), polytour::CriticalError);

    polytour::transport::Match match;
    match.id = 777;
    match.status = polytour::transport::Match::status_wait_participants();
    match.tournament_id = 777;
    match.tour = 1;
    EXPECT_THROW(matchRepo->addObj(match), polytour::CriticalError);
    EXPECT_NO_THROW(matchRepo->updateObj(match, match));
    EXPECT_NO_THROW(matchRepo->findObj({}));
    EXPECT_THROW(matchRepo->deleteObj(match), polytour::CriticalError);

    polytour::transport::TournamentParticipant participant;
    participant.id = 1;
    participant.tournament_id = 1;
    participant.participant_id = 1;
    EXPECT_THROW(tournamentParticipantsRepo->addObj(participant), polytour::CriticalError);
    EXPECT_THROW(tournamentParticipantsRepo->updateObj(participant, participant), polytour::CriticalError);
    EXPECT_NO_THROW(tournamentParticipantsRepo->findObj({}));
    EXPECT_NO_THROW(tournamentParticipantsRepo->deleteObj(participant));
}


TEST(db_integration_tests, testLeaderRole) {
    auto repoFactoryCreator = std::make_shared<polytour::db::repository::RepositoryFactoryCreator>();
    auto repoFactory = repoFactoryCreator->create(polytour::transport::Roles::Leader);
    auto userRepo = repoFactory->getUserRepository();
    auto tournamentRepo = repoFactory->getTournamentRepository();
    auto tournamentParticipantsRepo = repoFactory->getTournamentParticipantsRepository();
    auto matchRepo = repoFactory->getMatchRepository();

    polytour::transport::User user;
    user.id = 1;
    user.email = "email";
    user.name = "name";
    user.password = "pass";
    user.nickname = "nick";
    EXPECT_THROW(userRepo->addObj(user), polytour::CriticalError);

    EXPECT_NO_THROW(userRepo->findObj({}));
    auto newUser = user;
    newUser.nickname = "nick2";
    EXPECT_NO_THROW(userRepo->updateObj(user, newUser));
    EXPECT_NO_THROW(userRepo->deleteObj(newUser));

    polytour::transport::Tournament tournament;
    tournament.id = 777;
    tournament.name = "name";
    tournament.max_participants_num = 4;
    tournament.cur_participants_num = 1;
    tournament.status = polytour::transport::Tournament::status_wait_for_participants();
    tournament.organizer_id = 1;
    EXPECT_NO_THROW(tournamentRepo->addObj(tournament));
    EXPECT_NO_THROW(tournamentRepo->updateObj(tournament, tournament));
    EXPECT_NO_THROW(tournamentRepo->findObj({}));
    EXPECT_NO_THROW(tournamentRepo->deleteObj(tournament));

    polytour::transport::Match match;
    match.id = 777;
    match.status = polytour::transport::Match::status_wait_participants();
    match.tournament_id = 777;
    match.tour = 1;
    EXPECT_NO_THROW(matchRepo->addObj(match));
    EXPECT_NO_THROW(matchRepo->updateObj(match, match));
    EXPECT_NO_THROW(matchRepo->findObj({}));
    EXPECT_NO_THROW(matchRepo->deleteObj(match));

    polytour::transport::TournamentParticipant participant;
    participant.id = 1;
    participant.tournament_id = 1;
    participant.participant_id = 1;
    EXPECT_NO_THROW(tournamentParticipantsRepo->addObj(participant));
    EXPECT_NO_THROW(tournamentParticipantsRepo->updateObj(participant, participant));
    EXPECT_NO_THROW(tournamentParticipantsRepo->findObj({}));
    EXPECT_NO_THROW(tournamentParticipantsRepo->deleteObj(participant));
}