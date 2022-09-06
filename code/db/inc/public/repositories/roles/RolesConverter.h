//
// Created by denis on 22.08.2022.
//

#ifndef COURSEWORKDB_ROLESCONVERTER_H
#define COURSEWORKDB_ROLESCONVERTER_H

#include "Roles.h"
#include "IRole.h"
#include <memory>

namespace polytour::db::repository::roles {
    class RolesConverter {
    public:

        static std::shared_ptr<db::repository::roles::IRole> convert(transport::Roles role);

    };
}


#endif //COURSEWORKDB_ROLESCONVERTER_H
