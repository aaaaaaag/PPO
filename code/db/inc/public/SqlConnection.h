//
// Created by denis on 7/12/22.
//

#ifndef COURSEWORKDB_SQLCONNECTION_H
#define COURSEWORKDB_SQLCONNECTION_H

#include "IConnection.h"
#include "utility/ICommandSource.h"
#include "IDatabaseAdapter.h"
#include <memory>

namespace polytour::db {
    class SqlConnection: public IConnection {
    public:

        explicit SqlConnection(std::shared_ptr<IDatabaseAdapter> dbAdapter,
                               std::shared_ptr<utility::ICommandSource> commandSource);

        void addObj(const utility::FieldSet &fieldSet, const repository::Identity &identity) override;

        void deleteObj(const utility::FieldSet &fieldSet, const repository::Identity &identity) override;

        void updateObj(const utility::FieldSet& curFieldSet,
                                    const utility::FieldSet& newFieldSet,
                                    const repository::Identity& identity) override;

        std::vector<utility::FieldSet> searchObj(const utility::FieldSet &fieldSet,
                                                 const repository::Identity &identity) override;

        void init(const repository::Identity& identity) override;

        void commit() override;

    protected:

        std::shared_ptr<IDatabaseAdapter> _pDbAdapter;
        std::shared_ptr<utility::ICommandSource> _pCommandSource;
    };
}


#endif //COURSEWORKDB_SQLCONNECTION_H
