//
// Created by denis on 10.06.2022.
//

#ifndef CODE_IDATABASEADAPTER_H
#define CODE_IDATABASEADAPTER_H

#include <string>
#include <memory>
#include <optional>
#include "IDatabaseExecResult.h"

namespace polytour::db {
    /**
     * Low level database adapter for executing sql commands
     */
    class IDatabaseAdapter {
    public:

        virtual ~IDatabaseAdapter() = default;

        /**
         * Execute sql command
         * @param command sql command
         * @return Execution result
         */
        virtual std::shared_ptr<IDatabaseExecResult> executeSqlCommand(const std::string& command) = 0;

        /**
         * Commits transaction
         */
        virtual void commit() = 0;

    };
}


#endif //CODE_IDATABASEADAPTER_H
