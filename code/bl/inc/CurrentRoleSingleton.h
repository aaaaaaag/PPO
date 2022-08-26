//
// Created by denis on 23.08.2022.
//

#ifndef COURSEWORKDB_CURRENTROLESINGLETON_H
#define COURSEWORKDB_CURRENTROLESINGLETON_H

#include "Roles.h"
#include <memory>
#include <string>

namespace polytour::bl {
    class CurrentRoleSingleton {
    private:

        CurrentRoleSingleton() = default;

        CurrentRoleSingleton(const CurrentRoleSingleton &) = default;

        CurrentRoleSingleton &operator=(CurrentRoleSingleton &) = default;

    public:

        class RoleContainer {
        public:

            transport::Roles role = transport::Roles::Guest;

        };

        static RoleContainer* getInstance() {
            static std::unique_ptr<RoleContainer> _pInstance = std::make_unique<RoleContainer>();

            return _pInstance.get();
        }

    };
}

#endif //COURSEWORKDB_CURRENTROLESINGLETON_H
