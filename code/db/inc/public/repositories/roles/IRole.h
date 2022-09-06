//
// Created by denis on 05.08.2022.
//

#ifndef COURSEWORKDB_IROLE_H
#define COURSEWORKDB_IROLE_H

#include "User.h"

namespace polytour::db::repository {

    class IRepositoryFactory;

    namespace roles {
        class IRole {
        public:

            virtual ~IRole() = default;

        protected:

            friend IRepositoryFactory;
            virtual transport::User toUser() = 0;

        };
    }
}

#endif //COURSEWORKDB_IROLE_H
