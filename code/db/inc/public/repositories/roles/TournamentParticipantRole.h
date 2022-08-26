//
// Created by denis on 05.08.2022.
//

#ifndef COURSEWORKDB_TOURNAMENTPARTICIPANTROLE_H
#define COURSEWORKDB_TOURNAMENTPARTICIPANTROLE_H

#include "IRole.h"

namespace polytour::db::repository::roles {
    class TournamentParticipantRole: public IRole {
    protected:

        transport::User toUser() override;
    };
}


#endif //COURSEWORKDB_TOURNAMENTPARTICIPANTROLE_H
