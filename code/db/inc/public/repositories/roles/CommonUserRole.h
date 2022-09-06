//
// Created by denis on 23.08.2022.
//

#ifndef COURSEWORKDB_COMMONUSERROLE_H
#define COURSEWORKDB_COMMONUSERROLE_H

#include "IRole.h"

namespace polytour::db::repository::roles {
    class CommonUserRole: public IRole{
    protected:

        transport::User toUser() override;

    };
}

#endif //COURSEWORKDB_COMMONUSERROLE_H
