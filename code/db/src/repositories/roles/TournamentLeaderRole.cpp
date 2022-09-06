//
// Created by denis on 05.08.2022.
//

#include "public/repositories/roles/TournamentLeaderRole.h"

polytour::transport::User polytour::db::repository::roles::TournamentLeaderRole::toUser() {
    transport::User user;
    user.nickname = "leader";
    user.password = "leader";
    return user;
}
