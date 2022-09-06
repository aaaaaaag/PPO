//
// Created by denis on 10.06.2022.
//

#ifndef COURSEWORKDB_IDATABASEEXECRESULT_H
#define COURSEWORKDB_IDATABASEEXECRESULT_H

#include "public/utility/FieldSet.h"
#include <vector>

namespace polytour::db {
    /**
     * Represent result of executed command in database
     */
    class IDatabaseExecResult {
    public:

        virtual ~IDatabaseExecResult() = default;

        /**
         * Converts internal database result to medium-level result struct
         * @return medium-level result struct
         */
        virtual std::vector<utility::FieldSet> toFieldSetCollection() = 0;

    };
}

#endif //COURSEWORKDB_IDATABASEEXECRESULT_H
