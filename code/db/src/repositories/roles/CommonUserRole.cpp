//
// Created by denis on 23.08.2022.
//

#include "public/repositories/roles/CommonUserRole.h"

polytour::transport::User polytour::db::repository::roles::CommonUserRole::toUser() {
    transport::User user;
    user.nickname = "common_user";
    user.password = "common_user";
    return user;
}