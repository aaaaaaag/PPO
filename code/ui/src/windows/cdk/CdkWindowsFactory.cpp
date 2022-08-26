//
// Created by denis on 05.08.2022.
//

#include "windows/cdk/CdkWindowsFactory.h"

#include <utility>
#include "windows/cdk/AuthorizationWindow.h"
#include "windows/cdk/MainMenuWindow.h"
#include "windows/cdk/SignUpWindow.h"
#include "windows/cdk/UpdateUserWindow.h"
#include "windows/cdk/TournamentCreationWindow.h"
#include "windows/cdk/GuestTournamentWindow.h"
#include "windows/cdk/ParticipantTournamentWindow.h"
#include "windows/cdk/LeaderWaitTournamentWindow.h"
#include "windows/cdk/LeaderStartedTournamentWindow.h"
#include "CurrentRoleSingleton.h"

std::unique_ptr<polytour::ui::IWindow> polytour::ui::CdkWindowsFactory::createAuthorizationWindow() {
    return std::make_unique<cdk::AuthorizationWindow>(_pCoordinator.lock());
}

std::unique_ptr<polytour::ui::IWindow> polytour::ui::CdkWindowsFactory::createMainMenuWindow() {
    return std::make_unique<MainMenuWindow>(_pCoordinator.lock());
}

std::unique_ptr<polytour::ui::IWindow>
polytour::ui::CdkWindowsFactory::createTournamentWindow(const transport::Tournament& tournament) {
    auto currentUser = _pCoordinator.lock()->getMainAPI().userAPI()->currentUser();
    auto tournamentParticipants = _pCoordinator.lock()->getMainAPI().tournamentAPI()->getTournamentParticipants(tournament);
    bool isParticipant = false;
    for (const auto& participant: tournamentParticipants) {
        if (participant.id == currentUser.id)
            isParticipant = true;
    }

    if (currentUser.id == tournament.organizer_id) {
        bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::Leader;
        if (tournament.status == transport::Tournament::status_wait_for_participants())
            return std::make_unique<cdk::LeaderWaitTournamentWindow>(_pCoordinator.lock(), tournament);
        else if (tournament.status == transport::Tournament::status_started() ||
                tournament.status == transport::Tournament::status_finished())
            return std::make_unique<cdk::LeaderStartedTournamentWindow>(_pCoordinator.lock(), tournament);
        else
            throw std::logic_error("ban");
    }

    if (isParticipant) {
        bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::Participant;
        return std::make_unique<cdk::ParticipantTournamentWindow>(_pCoordinator.lock(), tournament);
    } else {
        bl::CurrentRoleSingleton::getInstance()->role = transport::Roles::CommonUser;
        return std::make_unique<cdk::GuestTournamentWindow>(_pCoordinator.lock(), tournament);
    }
}

std::unique_ptr<polytour::ui::IWindow> polytour::ui::CdkWindowsFactory::createUserInfoWindow() {
    return std::make_unique<cdk::UpdateUserWindow>(_pCoordinator.lock());
}

std::unique_ptr<polytour::ui::IWindow> polytour::ui::CdkWindowsFactory::createSignUpWindow() {
    return std::make_unique<cdk::SignUpWindow>(_pCoordinator.lock());
}

std::unique_ptr<polytour::ui::IWindow> polytour::ui::CdkWindowsFactory::createTournamentCreationWindow() {
    return std::make_unique<cdk::TournamentCreationWindow>(_pCoordinator.lock());
}

polytour::ui::CdkWindowsFactory::CdkWindowsFactory(const std::shared_ptr<ICoordinator>& coordinator):
_pCoordinator(coordinator){

}
