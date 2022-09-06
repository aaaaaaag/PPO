//
// Created by denis on 10.06.2022.
//

#ifndef COURSEWORKDB_FIELDSET_H
#define COURSEWORKDB_FIELDSET_H

#include <map>
#include <stdexcept>
#include "utility/BaseDefines.h"
#include "TableAbstractValue.h"

namespace polytour::db::utility {
    class FieldSet {
    public:
        typedef TableAbstractValue value_t;

        void addPair(const std::string& column, const TableAbstractValue& value) {
            row_values.emplace(column, value);
        };

        void updatePair(const std::string& column, const TableAbstractValue& value) {
            row_values.insert_or_assign(column, value);
        }

        [[nodiscard]] bool has_value_under_key(const std::string& column) const {
            return row_values.find(column) != row_values.end();
        }

        const TableAbstractValue& operator[] (const std::string& column) const{
            if (!has_value_under_key(column))
                throw std::invalid_argument("FieldSet hasn't value under key: " + column);
            return row_values.at(column);
        };

        TableAbstractValue& operator[] (const std::string& column){
            if (!has_value_under_key(column))
                throw std::invalid_argument("FieldSet hasn't value under key: " + column);
            return row_values.at(column);
        };

        // first string is column name, second is column value
        typedef std::map<column_name_t, value_t> table_values_t;

        table_values_t::iterator begin() {
            return row_values.begin();
        }

        [[nodiscard]] table_values_t::const_iterator begin() const {
            return row_values.begin();
        }

        table_values_t::iterator end() {
            return row_values.end();
        }

        [[nodiscard]] table_values_t::const_iterator end() const {
            return row_values.end();
        }

    private:

        table_values_t row_values;

    };
}

#endif //COURSEWORKDB_FIELDSET_H
