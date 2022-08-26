//
// Created by denis on 7/12/22.
//

#ifndef COURSEWORKDB_IREPOSITORYINITIALIZER_H
#define COURSEWORKDB_IREPOSITORYINITIALIZER_H

namespace polytour::db::repository {

    class IRepositoryInitializer {
    public:

        virtual ~IRepositoryInitializer() = default;

        virtual void init() = 0;

    };
}

#endif //COURSEWORKDB_IREPOSITORYINITIALIZER_H
