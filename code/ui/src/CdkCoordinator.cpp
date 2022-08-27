//
// Created by denis on 03.08.2022.
//

#include "CdkCoordinator.h"

#include <utility>
#include "facade/MainFacade.h"
#include "CurrentRoleSingleton.h"
#include "Logger.h"

std::optional<polytour::bl::ErrorObj>
polytour::ui::CdkCoordinator::authorize(const std::string &nick, const std::string &pass) {
    _pMainFacade->userAPI()->auth(nick, pass);
    if (!_pMainFacade->userAPI()->isError())
        bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::CommonUser;
    return _pMainFacade->userAPI()->getError();
}

polytour::bl::facade::IMainFacade &polytour::ui::CdkCoordinator::getMainAPI() {
    return *_pMainFacade;
}

void polytour::ui::CdkCoordinator::setFactory(std::shared_ptr<IWindowsFactory> factory) {
    _pWindowsFactory = std::move(factory);
    _pCurrentWindow = std::move(_pWindowsFactory->createAuthorizationWindow());
    _pCurrentWindow->init();
}

polytour::ui::CdkCoordinator::CdkCoordinator(std::shared_ptr<bl::facade::IMainFacade> mainFacade):
_pWindowsFactory(nullptr),
_pMainFacade(std::move(mainFacade)){
}

std::optional<polytour::bl::ErrorObj> polytour::ui::CdkCoordinator::signUp(polytour::transport::User user) {
    _pMainFacade->userAPI()->regNewUser(user);
    if (_pMainFacade->userAPI()->isError())
        return _pMainFacade->userAPI()->getError();
    bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::Guest;
    transport::Logger::debug("Change user current role to Guest");
    return std::nullopt;
}

std::optional<polytour::bl::ErrorObj> polytour::ui::CdkCoordinator::toSignIn() {
    transport::Logger::info("Go to sign in window");
    _pCurrentWindow->destroy();
    bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::Guest;
    transport::Logger::debug("Change user current role to Guest");
    _pCurrentWindow = _pWindowsFactory->createAuthorizationWindow();
    _pCurrentWindow->init();
    return std::nullopt;
}

std::optional<polytour::bl::ErrorObj> polytour::ui::CdkCoordinator::toSignUp() {
    transport::Logger::info("Go to sign up window");
    _pCurrentWindow->destroy();
    bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::Guest;
    transport::Logger::debug("Change user current role to Guest");
    _pCurrentWindow = _pWindowsFactory->createSignUpWindow();
    _pCurrentWindow->init();
    return std::nullopt;
}

std::optional<polytour::bl::ErrorObj> polytour::ui::CdkCoordinator::toMainMenu() {
    transport::Logger::info("Go to main menu window");
    _pCurrentWindow->destroy();
    _pCurrentWindow = nullptr;
    bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::CommonUser;
    transport::Logger::debug("Change user current role to CommonUser");
    _pCurrentWindow = _pWindowsFactory->createMainMenuWindow();
    _pCurrentWindow->init();
    return std::nullopt;
}

std::optional<polytour::bl::ErrorObj> polytour::ui::CdkCoordinator::toUpdateUser() {
    transport::Logger::info("Go to update user window");
    _pCurrentWindow->destroy();
    _pCurrentWindow = nullptr;
    bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::CommonUser;
    transport::Logger::debug("Change user current role to CommonUser");
    _pCurrentWindow = _pWindowsFactory->createUserInfoWindow();
    _pCurrentWindow->init();
    return std::nullopt;
}

std::optional<polytour::bl::ErrorObj> polytour::ui::CdkCoordinator::toTournamentCreation() {
    transport::Logger::info("Go to tournament creation window");
    _pCurrentWindow->destroy();
    bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::CommonUser;
    transport::Logger::debug("Change user current role to CommonUser");
    _pCurrentWindow = _pWindowsFactory->createTournamentCreationWindow();
    _pCurrentWindow->init();
    return std::nullopt;
}

std::optional<polytour::bl::ErrorObj> polytour::ui::CdkCoordinator::toTournament(const transport::Tournament& tournament) {
    transport::Logger::info("Go to tournament window");
    _pCurrentWindow->destroy();
    _pCurrentWindow = _pWindowsFactory->createTournamentWindow(tournament);
    _pCurrentWindow->init();
    return std::nullopt;
}
