//
// Created by denis on 22.08.2022.
//

#ifndef COURSEWORKDB_IREPOSITORYFACTORYCREATOR_H
#define COURSEWORKDB_IREPOSITORYFACTORYCREATOR_H

#include "IRepositoryFactory.h"
#include "Roles.h"

namespace polytour::db::repository {
    class IRepositoryFactoryCreator {
    public:
        virtual ~IRepositoryFactoryCreator() = default;

        virtual std::shared_ptr<IRepositoryFactory> create(transport::Roles role) = 0;
    };
}

#endif //COURSEWORKDB_IREPOSITORYFACTORYCREATOR_H
