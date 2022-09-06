//
// Created by denis on 25.08.2022.
//

#include "MockRepositoryFactoryCreator.h"
#include "MockRepositoryFactory.h"
#include "MockRepository.h"
#include "gtest/gtest.h"
#include "transactions/TournamentTransactionFactory.h"

using ::testing::Return;

namespace polytour::bl::tests::transactions {
    class TournamentTransactionFactoryTests : public ::testing::Test {
    protected:
        void SetUp() override {
            _pRepoFactory = std::make_shared<decltype(_pRepoFactory)::element_type>();
            _pRepoFactoryCreator = std::make_shared<decltype(_pRepoFactoryCreator)::element_type>();
            _pTournamentTransactionFactory = std::make_shared<decltype(_pTournamentTransactionFactory)::element_type>(
                    _pRepoFactoryCreator);
        }

        std::shared_ptr<db::tests::mocks::MockRepositoryFactoryCreator> _pRepoFactoryCreator;
        std::shared_ptr<transaction::TournamentTransactionFactory> _pTournamentTransactionFactory;
        std::shared_ptr<db::tests::mocks::MockRepositoryFactory> _pRepoFactory;
    };

    TEST_F(TournamentTransactionFactoryTests, create) {
        auto tournamentRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Tournament>>();
        EXPECT_CALL(*tournamentRepo, addObj);
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getTournamentRepository).WillOnce(Return(tournamentRepo.get()));
        transport::Tournament tournament;
        EXPECT_NO_THROW(_pTournamentTransactionFactory->create(tournament));
    }

    TEST_F(TournamentTransactionFactoryTests, erase) {
        auto tournamentRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Tournament>>();
        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        auto tournamentParticipantsRepo = std::make_shared<db::tests::mocks::MockRepository<transport::TournamentParticipant>>();

        std::vector<transport::Match> vMatches;
        transport::Match match;
        vMatches.emplace_back(match);

        EXPECT_CALL(*matchRepo, findObj).WillOnce(Return(vMatches));
        EXPECT_CALL(*matchRepo, deleteObj);

        std::vector<transport::TournamentParticipant> vParticipants;
        transport::TournamentParticipant participant;
        vParticipants.emplace_back(participant);
        EXPECT_CALL(*tournamentParticipantsRepo, findObj).WillOnce(Return(vParticipants));
        EXPECT_CALL(*tournamentParticipantsRepo, deleteObj);

        EXPECT_CALL(*tournamentRepo, deleteObj);

        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getTournamentRepository).WillOnce(Return(tournamentRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getTournamentParticipantsRepository).WillOnce(Return(tournamentParticipantsRepo.get()));
        transport::Tournament tournament;
        EXPECT_NO_THROW(_pTournamentTransactionFactory->erase(tournament));
    }

    TEST_F(TournamentTransactionFactoryTests, join) {
        auto tournamentRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Tournament>>();
        auto tournamentParticipantsRepo = std::make_shared<db::tests::mocks::MockRepository<transport::TournamentParticipant>>();

        EXPECT_CALL(*tournamentParticipantsRepo, addObj);

        EXPECT_CALL(*tournamentRepo, updateObj);

        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getTournamentRepository).WillOnce(Return(tournamentRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getTournamentParticipantsRepository).WillOnce(Return(tournamentParticipantsRepo.get()));
        transport::Tournament tournament;
        tournament.cur_participants_num = 1;
        tournament.max_participants_num = 2;
        tournament.status = transport::Tournament::status_wait_for_participants();
        EXPECT_NO_THROW(_pTournamentTransactionFactory->join(tournament));
    }

    TEST_F(TournamentTransactionFactoryTests, joinInvalidTournament) {
        transport::Tournament tournament;
        tournament.cur_participants_num = 2;
        tournament.max_participants_num = 2;
        tournament.status = transport::Tournament::status_started();
        EXPECT_NO_THROW(_pTournamentTransactionFactory->join(tournament));
    }

    TEST_F(TournamentTransactionFactoryTests, search) {
        auto tournamentRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Tournament>>();

        std::vector<transport::Tournament> vTournaments;
        EXPECT_CALL(*tournamentRepo, findObj).WillOnce(Return(vTournaments));
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getTournamentRepository).WillOnce(Return(tournamentRepo.get()));
        transport::Tournament::search_t tournament;
        EXPECT_TRUE(_pTournamentTransactionFactory->search(tournament).empty());
    }

    TEST_F(TournamentTransactionFactoryTests, leaveWhereUserIsParticipant1) {
        auto tournamentRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Tournament>>();
        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        auto tournamentParticipantsRepo = std::make_shared<db::tests::mocks::MockRepository<transport::TournamentParticipant>>();
        auto userRepo = std::make_shared<db::tests::mocks::MockRepository<transport::User>>();

        EXPECT_CALL(*tournamentRepo, updateObj);
        std::vector<polytour::transport::TournamentParticipant> vParticipants;
        transport::TournamentParticipant participant;
        vParticipants.emplace_back(participant);
        EXPECT_CALL(*tournamentParticipantsRepo, findObj).WillOnce(Return(vParticipants));
        EXPECT_CALL(*tournamentParticipantsRepo, deleteObj);

        std::vector<transport::Match> vMatches;
        transport::Match match1;
        match1.status = transport::Match::status_started();
        match1.participant_1_id = 1;
        match1.participant_2_id = 2;
        vMatches.emplace_back(match1);
        transport::Match match2;
        match2.status = transport::Match::status_wait_participants();
        vMatches.emplace_back(match2);

        std::vector<transport::Match> vEmptyMatches;

        EXPECT_CALL(*matchRepo, findObj).WillOnce(Return(vMatches))
                                        .WillOnce(Return(vMatches))
                                        .WillOnce(Return(vEmptyMatches));

        std::vector<transport::User> vUsers;
        transport::User user;
        user.id = 1;
        vUsers.emplace_back(user);
        EXPECT_CALL(*userRepo, findObj).WillOnce(Return(vUsers));
        EXPECT_CALL(*matchRepo, updateObj).Times(4);

        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getTournamentRepository).WillOnce(Return(tournamentRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get())).WillOnce(Return(matchRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getUserRepository).WillOnce(Return(userRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getTournamentParticipantsRepository).WillOnce(Return(tournamentParticipantsRepo.get()));
        transport::Tournament tournament;
        tournament.max_participants_num = 4;
        tournament.cur_participants_num = 2;
        tournament.id = 1;
        EXPECT_NO_THROW(_pTournamentTransactionFactory->leave(tournament));
    }

    TEST_F(TournamentTransactionFactoryTests, leaveWhereUserIsParticipant2) {
        auto tournamentRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Tournament>>();
        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        auto tournamentParticipantsRepo = std::make_shared<db::tests::mocks::MockRepository<transport::TournamentParticipant>>();
        auto userRepo = std::make_shared<db::tests::mocks::MockRepository<transport::User>>();

        EXPECT_CALL(*tournamentRepo, updateObj);
        std::vector<polytour::transport::TournamentParticipant> vParticipants;
        transport::TournamentParticipant participant;
        vParticipants.emplace_back(participant);
        EXPECT_CALL(*tournamentParticipantsRepo, findObj).WillOnce(Return(vParticipants));
        EXPECT_CALL(*tournamentParticipantsRepo, deleteObj);

        std::vector<transport::Match> vMatches;
        transport::Match match1;
        match1.status = transport::Match::status_started();
        match1.participant_1_id = 2;
        match1.participant_2_id = 1;
        vMatches.emplace_back(match1);
        transport::Match match2;
        match2.status = transport::Match::status_wait_participants();
        vMatches.emplace_back(match2);

        std::vector<transport::Match> vEmptyMatches;

        EXPECT_CALL(*matchRepo, findObj)
                .WillOnce(Return(vEmptyMatches))
                .WillOnce(Return(vMatches))
                .WillOnce(Return(vMatches));

        std::vector<transport::User> vUsers;
        transport::User user;
        user.id = 1;
        vUsers.emplace_back(user);
        EXPECT_CALL(*userRepo, findObj).WillOnce(Return(vUsers));
        EXPECT_CALL(*matchRepo, updateObj).Times(4);

        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getTournamentRepository).WillOnce(Return(tournamentRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get())).WillOnce(Return(matchRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getUserRepository).WillOnce(Return(userRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getTournamentParticipantsRepository).WillOnce(Return(tournamentParticipantsRepo.get()));
        transport::Tournament tournament;
        tournament.max_participants_num = 4;
        tournament.cur_participants_num = 2;
        tournament.id = 1;
        EXPECT_NO_THROW(_pTournamentTransactionFactory->leave(tournament));
    }

    TEST_F(TournamentTransactionFactoryTests, getParticipants) {
        auto tournamentParticipantsRepo = std::make_shared<db::tests::mocks::MockRepository<transport::TournamentParticipant>>();
        auto userRepo = std::make_shared<db::tests::mocks::MockRepository<transport::User>>();
        std::vector<transport::TournamentParticipant> vParticipants;
        transport::TournamentParticipant participant;
        vParticipants.emplace_back(participant);
        EXPECT_CALL(*tournamentParticipantsRepo, findObj).WillOnce(Return(vParticipants));

        std::vector<transport::User> vUsers;
        transport::User user;
        vUsers.emplace_back(user);
        EXPECT_CALL(*userRepo, findObj).WillOnce(Return(vUsers));
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getTournamentParticipantsRepository).WillOnce(Return(tournamentParticipantsRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getUserRepository).WillOnce(Return(userRepo.get()));
        transport::Tournament tournament;
        EXPECT_EQ(_pTournamentTransactionFactory->getParticipants(tournament).size(), vUsers.size());
    }

    TEST_F(TournamentTransactionFactoryTests, start) {
        auto tournamentRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Tournament>>();
        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        auto tournamentParticipantsRepo = std::make_shared<db::tests::mocks::MockRepository<transport::TournamentParticipant>>();
        auto userRepo = std::make_shared<db::tests::mocks::MockRepository<transport::User>>();

        EXPECT_CALL(*tournamentRepo, updateObj);

        std::vector<transport::TournamentParticipant> vParticipants;
        transport::TournamentParticipant participant;
        vParticipants.emplace_back(participant);
        vParticipants.emplace_back(participant);
        vParticipants.emplace_back(participant);
        vParticipants.emplace_back(participant);
        EXPECT_CALL(*tournamentParticipantsRepo, findObj).WillOnce(Return(vParticipants));

        std::vector<transport::User> vUsers1;
        transport::User user;
        user.id = 1;
        vUsers1.emplace_back(user);
        EXPECT_CALL(*userRepo, findObj).WillRepeatedly(Return(vUsers1));

        EXPECT_CALL(*matchRepo, addObj).Times(3);


        EXPECT_CALL(*_pRepoFactoryCreator, create).WillRepeatedly(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getTournamentParticipantsRepository).WillOnce(Return(tournamentParticipantsRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getUserRepository).WillRepeatedly(Return(userRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getTournamentRepository).WillOnce(Return(tournamentRepo.get()));
        transport::Tournament tournament;
        tournament.max_participants_num = 4;
        EXPECT_NO_THROW(_pTournamentTransactionFactory->start(tournament));
    }
}