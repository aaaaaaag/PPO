//
// Created by denis on 05.08.2022.
//

#include "public/repositories/roles/TournamentParticipantRole.h"

polytour::transport::User polytour::db::repository::roles::TournamentParticipantRole::toUser() {
    transport::User user;
    user.nickname = "participant";
    user.password = "participant";
    return user;
}
