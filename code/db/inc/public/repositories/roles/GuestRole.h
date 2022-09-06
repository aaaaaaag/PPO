//
// Created by denis on 05.08.2022.
//

#ifndef COURSEWORKDB_GUESTROLE_H
#define COURSEWORKDB_GUESTROLE_H

#include "IRole.h"

namespace polytour::db::repository::roles {
    class GuestRole: public IRole{
    protected:

        transport::User toUser() override;

    };
}

#endif //COURSEWORKDB_GUESTROLE_H
