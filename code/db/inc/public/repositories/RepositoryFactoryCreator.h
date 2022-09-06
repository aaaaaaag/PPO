//
// Created by denis on 22.08.2022.
//

#ifndef COURSEWORKDB_REPOSITORYFACTORYCREATOR_H
#define COURSEWORKDB_REPOSITORYFACTORYCREATOR_H

#include "IRepositoryFactoryCreator.h"

namespace polytour::db::repository {
    class RepositoryFactoryCreator: public IRepositoryFactoryCreator{
    public:

        std::shared_ptr<IRepositoryFactory> create(transport::Roles role) override;

    };
}


#endif //COURSEWORKDB_REPOSITORYFACTORYCREATOR_H
