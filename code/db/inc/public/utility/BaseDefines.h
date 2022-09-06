//
// Created by denis on 17.06.2022.
//

#ifndef COURSEWORKDB_BASEDEFINES_H
#define COURSEWORKDB_BASEDEFINES_H

#include <map>
#include <string>

namespace polytour {
namespace db {
namespace utility {

    enum class TableTypes{
        Int = 0,
        String,
        Serial,
        Null
    };

    enum class FieldMandatory {
        NotNull = 0,
        MayBeNull,
    };

    typedef std::string column_name_t;
}
}
}

#endif //COURSEWORKDB_BASEDEFINES_H
