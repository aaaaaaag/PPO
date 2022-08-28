//
// Created by denis on 28.08.2022.
//

#ifndef COURSEWORKDB_MYSQLADAPTER_H
#define COURSEWORKDB_MYSQLADAPTER_H

#include "public/IDatabaseAdapter.h"
#include <memory>

namespace polytour::db {
    class MySqlAdapter : public IDatabaseAdapter {
    public:

        explicit MySqlAdapter(const polytour::db::utility::FieldSet& connInfo);

        ~MySqlAdapter() override;

        std::shared_ptr<IDatabaseExecResult> executeSqlCommand(const std::string& command) override;

        void commit() override;

    private:

        class MySqlAdapterImpl;
        std::unique_ptr<MySqlAdapterImpl> _pImpl;

    };
}


#endif //COURSEWORKDB_MYSQLADAPTER_H
