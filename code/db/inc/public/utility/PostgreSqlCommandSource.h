//
// Created by denis on 17.06.2022.
//

#ifndef COURSEWORKDB_POSTGRESQLCOMMANDSOURCE_H
#define COURSEWORKDB_POSTGRESQLCOMMANDSOURCE_H

#include "ICommandSource.h"
#include "repositories/Identity.h"

namespace polytour {
namespace db {
namespace utility {
    class PostgreSqlCommandSource: public ICommandSource {
    public:

        std::string addObj(const FieldSet &fieldSet, repository::Identity repoIdentity) override;

        std::string deleteObj(const FieldSet &fieldSet, repository::Identity repoIdentity) override;

        std::string updateObj(const FieldSet &curFieldSet,
                              const FieldSet &newFieldSet,
                              repository::Identity repoIdentity) override;

        std::string searchObj(const FieldSet& conditionFieldSet,
                              repository::Identity repoIdentity) override;

        std::string initTable(const repository::Identity &identity) override;

    private:

        static void checkFieldSetVsIdentity(const FieldSet &fieldSet, const repository::Identity& repoIdentity) ;

        static std::string buildColumnsList(const repository::Identity& repoIdentity);

        static std::string buildValuesList(const FieldSet& fieldSet, const repository::Identity& repoIdentity);

        static std::string buildConditionList(const FieldSet& fieldSet, const repository::Identity& repoIdentity) ;

        static std::string buildSetList(const FieldSet& fieldSet, const repository::Identity& repoIdentity) ;

        static std::string valueToSQLCommandView(
                const FieldSet::value_t& value, const std::string& column_name,
                TableTypes columnType, FieldMandatory mandatory);

    };
}
}
}



#endif //COURSEWORKDB_POSTGRESQLCOMMANDSOURCE_H
