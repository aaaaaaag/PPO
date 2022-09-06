//
// Created by denis on 04.08.2022.
//

#include "facade/UserFacade.h"

#include <utility>
#include "AuthUserSingleton.h"
#include "transactions/UserTransactionFactory.h"
#include "repositories/roles/GuestRole.h"
#include "NotCriticalError.h"

void polytour::bl::facade::UserFacade::auth(const std::string &nick, const std::string &pass) {
    processError([this, pass, nick]() {
        auto users = _pTransactionFactory->searchUsers({.password_ = pass, .nickname_ = nick});
        if (users.size() != 1)
            throw polytour::NotCriticalError("Failed authorization");

        polytour::bl::AuthUserSingleton::authorize(users[0]);
    });
}

std::vector<polytour::transport::User> polytour::bl::facade::UserFacade::getUsers(polytour::transport::User::search_t search) {
    std::vector<transport::User> result;
    processError([this, search, &result]() {
        result = _pTransactionFactory->searchUsers(search);
    });
    return result;
}

void polytour::bl::facade::UserFacade::deleteUser(const polytour::transport::User &user) {
    processError([this, user]() {
        _pTransactionFactory->deleteUser(user);
    });
}

void polytour::bl::facade::UserFacade::regNewUser(const polytour::transport::User &user) {
    processError([this, user]() {
        if (user.nickname.empty())
            throw polytour::NotCriticalError("Nickname field doesn't passed");
        auto users = _pTransactionFactory->searchUsers({.nickname_ = user.nickname});
        if (!users.empty())
            throw polytour::NotCriticalError("Nickname is already in use. Pass another");
        if (user.password.empty())
            throw polytour::NotCriticalError("Password field doesn't passed");
        if (user.name.empty())
            throw polytour::NotCriticalError("Name field doesn't passed");
        if (user.email.empty())
            throw polytour::NotCriticalError("Email field doesn't passed");
        _pTransactionFactory->addUser(user);
    });
}

void polytour::bl::facade::UserFacade::updateUser(const polytour::transport::User &user) {
    auto curUser = *AuthUserSingleton::getInstance();
    processError([this, user, curUser]() {
        if (user.nickname.empty())
            throw polytour::NotCriticalError("Nickname field doesn't passed");
        auto users = _pTransactionFactory->searchUsers({.nickname_ = user.nickname});
        if (!users.empty() && user.nickname != curUser.nickname)
            throw polytour::NotCriticalError("Nickname is already in use. Pass another");
        if (user.password.empty())
            throw polytour::NotCriticalError("Password field doesn't passed");
        if (user.name.empty())
            throw polytour::NotCriticalError("Name field doesn't passed");
        if (user.email.empty())
            throw polytour::NotCriticalError("Email field doesn't passed");
        auto newUser = user;
        newUser.id = curUser.id;
        _pTransactionFactory->updateUser(curUser, newUser);
    });
}

polytour::transport::User polytour::bl::facade::UserFacade::currentUser() {
    transport::User result;
    auto authUserNick = AuthUserSingleton::getInstance()->nickname;
    processError([this, authUserNick, &result]() {
        auto found = _pTransactionFactory->searchUsers({.nickname_ = authUserNick});
        if (found.size() != 1) {
            throw CriticalError("Found more than one user with the same nickname: " + authUserNick);
        }
        result = found[0];
    });
    return result;
}

polytour::bl::facade::UserFacade::UserFacade(std::shared_ptr<transaction::IUserTransactionFactory> transactionFactory):
        _pTransactionFactory(std::move(transactionFactory)){}
