//
// Created by denis on 22.08.2022.
//

#ifndef COURSEWORKDB_ROLES_H
#define COURSEWORKDB_ROLES_H

namespace polytour::transport {
    enum class Roles {
        Guest = 0,
        CommonUser,
        Participant,
        Leader
    };
}

#endif //COURSEWORKDB_ROLES_H
