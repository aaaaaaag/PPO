//
// Created by denis on 17.06.2022.
//

#ifndef COURSEWORKDB_IDENTITY_H
#define COURSEWORKDB_IDENTITY_H

#include <string>
#include <tuple>
#include <map>
#include "utility/BaseDefines.h"

namespace polytour {
namespace db {
namespace repository {

    typedef struct ColumnDescription {
        utility::TableTypes type;
        utility::FieldMandatory mandatory;
    } ColumnDescription;

    typedef struct Identity {
        typedef ColumnDescription column_description_t;
        typedef std::map<utility::column_name_t, column_description_t> table_columns_t;

        std::string tableName;
        table_columns_t tableColumns;
    } Identity;
}
}
}

#endif //COURSEWORKDB_IDENTITY_H
