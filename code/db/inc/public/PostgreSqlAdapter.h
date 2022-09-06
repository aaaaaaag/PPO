//
// Created by denis on 10.06.2022.
//

#ifndef CODE_POSTGRESQLADAPTER_H
#define CODE_POSTGRESQLADAPTER_H

#include "public/IDatabaseAdapter.h"
#include <memory>

namespace polytour::db {
    class PostgreSqlAdapter : public IDatabaseAdapter {
    public:

        explicit PostgreSqlAdapter(const polytour::db::utility::FieldSet& connInfo);

        ~PostgreSqlAdapter() override;

        std::shared_ptr<IDatabaseExecResult> executeSqlCommand(const std::string& command) override;

        void commit() override;

    private:

        class PostgreSqlAdapterImpl;
        std::unique_ptr<PostgreSqlAdapterImpl> _pImpl;

    };
}


#endif //CODE_POSTGRESQLADAPTER_H
