//
// Created by denis on 28.08.2022.
//

#ifndef COURSEWORKDB_CPPCONNRESULTCONVERTER_H
#define COURSEWORKDB_CPPCONNRESULTCONVERTER_H


#include "utility/FieldSet.h"
#include "cppconn/resultset.h"
#include "Logger.h"
#include "CriticalError.h"

namespace polytour {
    namespace db{
        namespace priv {
            namespace cppconn {
                class ResultConverter {
                public:
                    static std::vector<utility::FieldSet> toFieldSet(sql::ResultSet* result) {
                        std::vector<utility::FieldSet> resultFieldSet;
                        size_t currentRowId = 0;

                        while(result->next()) {
                            utility::FieldSet resultRowElems;
                            for (int i = 1; i <= result->getMetaData()->getColumnCount(); i++) {
                                std::string colName = result->getMetaData()->getColumnName(i).c_str();
                                auto type = result->getMetaData()->getColumnType(i);
                                transport::Logger::info("Column: " + colName);
                                transport::Logger::info("Type: " + std::to_string(type));
                                if (result->isNull(i)) {
                                    resultRowElems.addPair(colName, utility::TableAbstractValue(
                                            std::nullopt, utility::TableTypes::Null));
                                }
                                else if (type == 15) {
                                    resultRowElems.addPair(colName, utility::TableAbstractValue(
                                            std::string(result->getString(i)), utility::TableTypes::String));
                                } else if (type == 5) {
                                    resultRowElems.addPair(colName, utility::TableAbstractValue(
                                            result->getInt(i), utility::TableTypes::Int));
                                } else {
                                    throw polytour::CriticalError("Unexpected result type: " + result->getMetaData()->getColumnTypeName(i) +
                                    "Result type id: " + std::to_string(type));
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

#endif //COURSEWORKDB_CPPCONNRESULTCONVERTER_H
