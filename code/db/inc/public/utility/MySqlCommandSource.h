//
// Created by denis on 28.08.2022.
//

#ifndef COURSEWORKDB_MYSQLCOMMANDSOURCE_H
#define COURSEWORKDB_MYSQLCOMMANDSOURCE_H


#include "ICommandSource.h"
#include "repositories/Identity.h"

namespace polytour::db::utility {
    class MySqlCommandSource: public ICommandSource {
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


#endif //COURSEWORKDB_MYSQLCOMMANDSOURCE_H
