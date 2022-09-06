//
// Created by denis on 7/12/22.
//

#ifndef COURSEWORKDB_ICONNECTION_H
#define COURSEWORKDB_ICONNECTION_H

#include "utility/FieldSet.h"
#include "repositories/Identity.h"
#include <vector>

namespace polytour::db {
    class IConnection {
    public:

        virtual ~IConnection() = default;

        virtual void addObj(const utility::FieldSet& fieldSet, const repository::Identity& identity) = 0;

        virtual void deleteObj(const utility::FieldSet& fieldSet, const repository::Identity& identity) = 0;

        virtual void updateObj(const utility::FieldSet& curFieldSet,
                                            const utility::FieldSet& newFieldSet,
                                            const repository::Identity& identity) = 0;

        virtual std::vector<utility::FieldSet> searchObj(const utility::FieldSet& conditionFieldSet,
                                                         const repository::Identity& identity) = 0;

        virtual void init(const repository::Identity& identity) = 0;

        virtual void commit() = 0;

    };
}

#endif //COURSEWORKDB_ICONNECTION_H
