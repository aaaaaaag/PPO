//
// Created by denis on 22.08.2022.
//

#ifndef COURSEWORKDB_ACTIVEROLESINGLETON_H
#define COURSEWORKDB_ACTIVEROLESINGLETON_H

#include "repositories/roles/IRole.h"
#include <memory>

namespace polytour::db {
    class ActiveRoleSingleton {
    private:

        ActiveRoleSingleton() = default;
        ActiveRoleSingleton(const ActiveRoleSingleton&) = default;
        ActiveRoleSingleton& operator = (ActiveRoleSingleton&) = default;

    public:

        static repository::roles::IRole* getInstance() {
            static std::unique_ptr<repository::roles::IRole> _pInstance = nullptr;
            return _pInstance.get();
        }
    };
}

#endif //COURSEWORKDB_ACTIVEROLESINGLETON_H
