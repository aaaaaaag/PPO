//
// Created by denis on 17.06.2022.
//

#ifndef COURSEWORKDB_ICOMMANDSOURCE_H
#define COURSEWORKDB_ICOMMANDSOURCE_H

#include "FieldSet.h"
#include "repositories/Identity.h"
#include <vector>

namespace polytour {
namespace db {
namespace utility {
    class ICommandSource {
    public:

        virtual ~ICommandSource() = default;

        virtual std::string addObj(const FieldSet& fieldSet, repository::Identity repoIdentity) = 0;

        virtual std::string deleteObj(const FieldSet& fieldSet, repository::Identity repoIdentity) = 0;

        virtual std::string updateObj(const FieldSet &curFieldSet,
                                      const FieldSet &newFieldSet,
                                      repository::Identity repoIdentity) = 0;

        virtual std::string searchObj(const FieldSet& fieldSet, repository::Identity repoIdentity) = 0;

        virtual std::string initTable(const repository::Identity& identity) = 0;

    };
}
}
}

#endif //COURSEWORKDB_ICOMMANDSOURCE_H
