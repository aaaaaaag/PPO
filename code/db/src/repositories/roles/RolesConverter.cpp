//
// Created by denis on 22.08.2022.
//

#include "public/repositories/roles/RolesConverter.h"
#include "public/repositories/roles/GuestRole.h"
#include "public/repositories/roles/TournamentParticipantRole.h"
#include "public/repositories/roles/TournamentLeaderRole.h"
#include "public/repositories/roles/CommonUserRole.h"

std::shared_ptr<polytour::db::repository::roles::IRole>
polytour::db::repository::roles::RolesConverter::convert(polytour::transport::Roles role) {
    switch (role) {
        case polytour::transport::Roles::Guest:
            return std::make_unique<db::repository::roles::GuestRole>();
        case polytour::transport::Roles::Participant:
            return std::make_unique<db::repository::roles::TournamentParticipantRole>();
        case polytour::transport::Roles::Leader:
            return std::make_unique<db::repository::roles::TournamentLeaderRole>();
        case polytour::transport::Roles::CommonUser:
            return std::make_unique<db::repository::roles::CommonUserRole>();
        default:
            throw polytour::CriticalError("Passed invalid role to roles converter");
    }
}
