//
// Created by denis on 05.08.2022.
//

#ifndef COURSEWORKDB_TOURNAMENTLEADERROLE_H
#define COURSEWORKDB_TOURNAMENTLEADERROLE_H

#include "IRole.h"

namespace polytour::db::repository::roles {
    class TournamentLeaderRole: public IRole{
    protected:

        transport::User toUser() override;

    };
}

#endif //COURSEWORKDB_TOURNAMENTLEADERROLE_H
