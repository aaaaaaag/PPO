//
// Created by denis on 05.08.2022.
//

#include "public/repositories/roles/GuestRole.h"

polytour::transport::User polytour::db::repository::roles::GuestRole::toUser() {
    transport::User user;
    user.nickname = "guest";
    user.password = "guest";
    return user;
}
