//
// Created by denis on 25.08.2022.
//

#include "MockRepositoryFactoryCreator.h"
#include "MockRepositoryFactory.h"
#include "MockRepository.h"
#include "gtest/gtest.h"
#include "transactions/UserTransactionFactory.h"

using ::testing::Return;

namespace polytour::bl::tests::transactions {
    class UserTransactionFactoryTests: public ::testing::Test {
    public:

    protected:
        void SetUp() override {
            _pRepoFactory = std::make_shared<decltype(_pRepoFactory)::element_type>();
            _pRepoFactoryCreator = std::make_shared<decltype(_pRepoFactoryCreator)::element_type>();
            _pUserTransactionFactory = std::make_shared<decltype(_pUserTransactionFactory)::element_type>(_pRepoFactoryCreator);
        }

        std::shared_ptr<db::tests::mocks::MockRepositoryFactoryCreator> _pRepoFactoryCreator;
        std::shared_ptr<transaction::UserTransactionFactory> _pUserTransactionFactory;
        std::shared_ptr<db::tests::mocks::MockRepositoryFactory> _pRepoFactory;
    };

    TEST_F(UserTransactionFactoryTests, addUser) {
        auto userRepo = std::make_shared<db::tests::mocks::MockRepository<transport::User>>();
        EXPECT_CALL(*userRepo, addObj);
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getUserRepository).WillOnce(Return(userRepo.get()));
        transport::User user;
        EXPECT_NO_THROW(_pUserTransactionFactory->addUser(user));
    }

    TEST_F(UserTransactionFactoryTests, updateUser) {
        auto userRepo = std::make_shared<db::tests::mocks::MockRepository<transport::User>>();
        EXPECT_CALL(*userRepo, updateObj);
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getUserRepository).WillOnce(Return(userRepo.get()));
        transport::User curUser, newUser;
        EXPECT_NO_THROW(_pUserTransactionFactory->updateUser(curUser, newUser));
    }

    TEST_F(UserTransactionFactoryTests, searchUsers) {
        std::vector<transport::User> _vResult;
        auto userRepo = std::make_shared<db::tests::mocks::MockRepository<transport::User>>();
        EXPECT_CALL(*userRepo, findObj).WillOnce(Return(_vResult));
        EXPECT_CALL(*_pRepoFactoryCreator, create).WillOnce(Return(_pRepoFactory));
        EXPECT_CALL(*_pRepoFactory, getUserRepository).WillOnce(Return(userRepo.get()));
        transport::User::search_t user;
        EXPECT_TRUE(_pUserTransactionFactory->searchUsers(user).empty());
    }
}