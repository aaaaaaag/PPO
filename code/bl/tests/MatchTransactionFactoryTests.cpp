//
// Created by denis on 25.08.2022.
//

#include "MockRepositoryFactoryCreator.h"
#include "MockRepositoryFactory.h"
#include "MockRepository.h"
#include "gtest/gtest.h"
#include "transactions/MatchTransactionFactory.h"

using ::testing::Return;

namespace polytour::bl::tests::transactions {
    class MatchTransactionFactoryTests : public ::testing::Test {
    protected:
        void SetUp() override {
            _pRepoFactory = std::make_shared<decltype(_pRepoFactory)::element_type>();
            _pRepoFactoryCreator = std::make_shared<decltype(_pRepoFactoryCreator)::element_type>();
            _pMatchTransactionFactory = std::make_shared<decltype(_pMatchTransactionFactory)::element_type>(
                    _pRepoFactoryCreator);
        }

        std::shared_ptr<db::tests::mocks::MockRepositoryFactoryCreator> _pRepoFactoryCreator;
        std::shared_ptr<transaction::MatchTransactionFactory> _pMatchTransactionFactory;
        std::shared_ptr<db::tests::mocks::MockRepositoryFactory> _pRepoFactory;
    };

    TEST_F(MatchTransactionFactoryTests, create) {
        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        EXPECT_CALL(*matchRepo, addObj);
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        transport::Match match;
        EXPECT_NO_THROW(_pMatchTransactionFactory->create(match));
    }

    TEST_F(MatchTransactionFactoryTests, erase) {
        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        EXPECT_CALL(*matchRepo, deleteObj);
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        transport::Match match;
        EXPECT_NO_THROW(_pMatchTransactionFactory->erase(match));
    }

    TEST_F(MatchTransactionFactoryTests, getMatches) {
        std::vector<transport::Match> _vResult;

        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        EXPECT_CALL(*matchRepo, findObj).WillOnce(Return(_vResult));
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        transport::Match::search_t match;
        EXPECT_TRUE(_pMatchTransactionFactory->getMatches(match).empty());
    }

    TEST_F(MatchTransactionFactoryTests, updateMatch) {
        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        EXPECT_CALL(*matchRepo, updateObj);
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        transport::Match curMatch, newMatch;
        EXPECT_NO_THROW(_pMatchTransactionFactory->updateMatch(curMatch, newMatch));
    }

    TEST_F(MatchTransactionFactoryTests, finishNotLastMatch1) {
        std::vector<transport::Match> _vNextTourMatches;
        transport::Match nextTourMatch;
        nextTourMatch.participant_1_id = 5;
        _vNextTourMatches.emplace_back(nextTourMatch);

        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        EXPECT_CALL(*matchRepo, updateObj).Times(2);
        EXPECT_CALL(*matchRepo, findObj).WillOnce(Return(_vNextTourMatches));
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        transport::Match match;
        match.participant_1_id = 1;
        transport::User user;
        user.id = 1;
        EXPECT_NO_THROW(_pMatchTransactionFactory->finish(match, user));
    }

    TEST_F(MatchTransactionFactoryTests, finishNotLastMatch2) {
        std::vector<transport::Match> _vNextTourMatches;
        transport::Match nextTourMatch;
        nextTourMatch.participant_2_id = 5;
        _vNextTourMatches.emplace_back(nextTourMatch);

        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        EXPECT_CALL(*matchRepo, updateObj).Times(2);
        EXPECT_CALL(*matchRepo, findObj).WillOnce(Return(_vNextTourMatches));
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        transport::Match match;
        match.participant_1_id = 10;
        match.participant_2_id = 1;
        transport::User user;
        user.id = 1;
        EXPECT_NO_THROW(_pMatchTransactionFactory->finish(match, user));
    }

    TEST_F(MatchTransactionFactoryTests, finishLastMatch) {
        std::vector<transport::Match> _vNextTourMatches;
        std::vector<transport::Tournament> _vTournaments;
        transport::Tournament tournament;
        _vTournaments.emplace_back(tournament);

        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        auto tournamentRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Tournament>>();
        EXPECT_CALL(*matchRepo, updateObj);
        EXPECT_CALL(*matchRepo, findObj).WillOnce(Return(_vNextTourMatches));
        EXPECT_CALL(*tournamentRepo, findObj).WillOnce(Return(_vTournaments));
        EXPECT_CALL(*tournamentRepo, updateObj);

        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        EXPECT_CALL(*_pRepoFactory, getTournamentRepository).WillOnce(Return(tournamentRepo.get()));
        transport::Match match;
        match.participant_1_id = 1;
        transport::User user;
        user.id = 1;
        EXPECT_NO_THROW(_pMatchTransactionFactory->finish(match, user));
    }


    TEST_F(MatchTransactionFactoryTests, finishMatchErrorInvalidPassedUser) {
        auto matchRepo = std::make_shared<db::tests::mocks::MockRepository<transport::Match>>();
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getMatchRepository).WillOnce(Return(matchRepo.get()));
        transport::Match match;
        match.participant_1_id = 5;
        match.participant_2_id = 7;
        transport::User user;
        user.id = 1;
        EXPECT_THROW(_pMatchTransactionFactory->finish(match, user), polytour::CriticalError);
    }
}