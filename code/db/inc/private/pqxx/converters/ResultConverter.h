//
// Created by denis on 16.06.2022.
//

#ifndef COURSEWORKDB_RESULTCONVERTER_H
#define COURSEWORKDB_RESULTCONVERTER_H

#include "utility/FieldSet.h"
#include "pqxx/pqxx"

namespace polytour {
namespace db{
namespace priv {
namespace libpqxx {
    class ResultConverter {
    public:
        static std::vector<utility::FieldSet> toFieldSet(const pqxx::result& result) {
            std::vector<utility::FieldSet> resultFieldSet;
            size_t currentRowId = 0;
            for (auto row = result.begin(); row != result.end(); row++, currentRowId++) {
                utility::FieldSet resultRowElems;
                for (auto el = row.begin(); el != row.end(); el++) {
                    //TODO THIS IS FUCKING SHIT AND NEED TO FIX IT IN THE FUTURE
                    auto type = el.type();
                    auto name = el.name();
                    auto value = el.c_str();
                    auto is_null = el.is_null();
                    if (el.type() == 25) {
                        if (el.is_null())
                            resultRowElems.addPair(el.name(), utility::TableAbstractValue(
                                    std::nullopt, utility::TableTypes::Null));
                        else
                            resultRowElems.addPair(el.name(), utility::TableAbstractValue(
                                    std::string(el.c_str()), utility::TableTypes::String));
                    }
                    if (el.type() == 23) {
                        if (el.is_null())
                            resultRowElems.addPair(el.name(), utility::TableAbstractValue(
                                    std::nullopt, utility::TableTypes::Null));
                        else
                            resultRowElems.addPair(el.name(), utility::TableAbstractValue(
                                    std::stoi(el.c_str()), utility::TableTypes::Int));
                    }
                }
                resultFieldSet.emplace_back(resultRowElems);
            }
            return resultFieldSet;
        }
    };
}
}
}
}

#endif //COURSEWORKDB_RESULTCONVERTER_H
