//
// Created by denis on 17.06.2022.
//

#include "public/utility/PostgreSqlCommandSource.h"
#include "CriticalError.h"

std::string polytour::db::utility::PostgreSqlCommandSource::addObj(const polytour::db::utility::FieldSet &fieldSet,
                                                                   repository::Identity repoIdentity) {
    checkFieldSetVsIdentity(fieldSet, repoIdentity);
    std::string commandNamePart = "insert into";
    std::string tableName = repoIdentity.tableName;
    std::string tableColumns = buildColumnsList(repoIdentity);
    std::string tableValues = "values " + buildValuesList(fieldSet, repoIdentity);
    return commandNamePart + " " + tableName + " " + tableColumns + " " + tableValues;
}

std::string polytour::db::utility::PostgreSqlCommandSource::deleteObj(const polytour::db::utility::FieldSet &fieldSet,
                                                                      repository::Identity repoIdentity) {
    checkFieldSetVsIdentity(fieldSet, repoIdentity);
    std::string commandNamePart = "delete from";
    std::string tableName = repoIdentity.tableName;
    std::string deleteWherePart = "where";
    std::string conditionList = buildConditionList(fieldSet, repoIdentity);
    return commandNamePart + " " + tableName + " " + deleteWherePart + " " + conditionList;
}

std::string polytour::db::utility::PostgreSqlCommandSource::updateObj(const FieldSet &curFieldSet,
                                                                      const FieldSet &newFieldSet,
                                                                      repository::Identity repoIdentity) {
    checkFieldSetVsIdentity(curFieldSet, repoIdentity);
    checkFieldSetVsIdentity(newFieldSet, repoIdentity);
    std::string commandNamePart = "update";
    std::string tableName = repoIdentity.tableName;
    std::string setCommand = "set";
    std::string setValues = buildSetList(newFieldSet, repoIdentity);
    std::string deleteWherePart = "where";
    std::string conditionList = buildConditionList(curFieldSet, repoIdentity);
    return commandNamePart + " " + tableName + " " +
            setCommand + " " + setValues + " " +
            deleteWherePart + " " + conditionList;
}


std::string polytour::db::utility::PostgreSqlCommandSource::searchObj(
        const polytour::db::utility::FieldSet &conditionFieldSet, repository::Identity repoIdentity) {
    checkFieldSetVsIdentity(conditionFieldSet, repoIdentity);
    std::string commandNamePart = "select";
    std::string fromCommand = "from";
    std::string tableName = repoIdentity.tableName;

    std::string resultCommand = commandNamePart + " " + "*" + " " +
                                fromCommand + " " + tableName ;

    std::string wherePart = "where";
    std::string conditionList = buildConditionList(conditionFieldSet, repoIdentity);

    if (!conditionList.empty())
        resultCommand += " " + wherePart + " " + conditionList;
    return resultCommand;
}

std::string
polytour::db::utility::PostgreSqlCommandSource::initTable(const polytour::db::repository::Identity &identity) {
    std::string commandNamePart = "create table if not exists";
    std::string tableName = identity.tableName;
    std::string columns;
    columns += "(";
    for (const auto& column: identity.tableColumns) {
        std::string curColumn;
        curColumn += column.first;
        curColumn += " ";
        switch (column.second.type) {
            case TableTypes::Int:
                curColumn += "int"; break;
            case TableTypes::String:
                curColumn += "text"; break;
            case TableTypes::Serial:
                curColumn += "serial"; break;
            case TableTypes::Null:
            default:
                throw polytour::CriticalError("Table column has invalid type");
        }
        switch (column.second.mandatory) {
            case FieldMandatory::NotNull:
                curColumn += " not null"; break;
            default:
                break;
        }
        curColumn += ", ";
        columns += curColumn;
    }
    columns.erase(columns.size() - 2);
    columns += ")";
    return commandNamePart + " " + tableName + " " + columns;
}

std::string polytour::db::utility::PostgreSqlCommandSource::buildValuesList(const FieldSet& fieldSet,
                                                                            const repository::Identity& repoIdentity) {
    std::string result = "(";
    for (const auto& column: repoIdentity.tableColumns) {
        if (column.second.type == TableTypes::Serial)
            continue;
        auto columnName = column.first;
        if (fieldSet.has_value_under_key(columnName))
            result += valueToSQLCommandView(fieldSet[columnName], columnName,
                                        column.second.type, column.second.mandatory);
        else
            result += valueToSQLCommandView(TableAbstractValue(std::nullopt), columnName,
                                            column.second.type, column.second.mandatory);
        result += ", ";
    }
    result.erase(result.size() - 2);
    result += ")";
    return result;
}

std::string
polytour::db::utility::PostgreSqlCommandSource::buildConditionList(
        const FieldSet& fieldSet,
        const repository::Identity& repoIdentity) {
    std::string conditionList;
    auto tableColumns = repoIdentity.tableColumns;
    for (const auto& value: fieldSet) {
        auto columnName = value.first;
        auto columnIdentity = tableColumns[columnName];
        conditionList += columnName;
        if (value.second.isNull())
            conditionList += " is ";
        else
            conditionList += " = ";
        conditionList += valueToSQLCommandView(value.second, columnName,
                                               columnIdentity.type, columnIdentity.mandatory);
        conditionList += " and ";
    }
    if (!conditionList.empty())
        conditionList.erase(conditionList.size() - 5);
    return conditionList;
}


std::string polytour::db::utility::PostgreSqlCommandSource::buildSetList(const FieldSet& fieldSet,
                                                                         const repository::Identity& repoIdentity) {
    std::string setList;
    auto tableColumns = repoIdentity.tableColumns;
    for (const auto& value: repoIdentity.tableColumns) {
        auto columnName = value.first;
        auto columnIdentity = tableColumns[columnName];
        setList += columnName;
        setList += " = ";
        if (fieldSet.has_value_under_key(columnName))
            setList += valueToSQLCommandView(fieldSet[columnName], columnName,
                                               columnIdentity.type, columnIdentity.mandatory);
        else
            setList += valueToSQLCommandView(TableAbstractValue(std::nullopt), columnName,
                                             columnIdentity.type, columnIdentity.mandatory);
        setList += ", ";
    }
    setList.erase(setList.size() - 2);
    return setList;
}

std::string polytour::db::utility::PostgreSqlCommandSource::valueToSQLCommandView(
        const FieldSet::value_t& value, const std::string& column_name, TableTypes columnType, FieldMandatory mandatory) {
    std::string stringExpectedType;
    switch (columnType) {
        case TableTypes::Serial:
        case TableTypes::Int:
            if (value.isInt()) return std::to_string(value.toInt());
            else stringExpectedType = "int";
            break;
        case TableTypes::String:
            if (value.isString()) return "'" + value.toString() + "'";
            else stringExpectedType = "string";
            break;
        default:
            throw polytour::CriticalError("Illegal expected value type for table column");
    }
    if (mandatory == FieldMandatory::MayBeNull) {
        if (value.isNull()) return "null";
    }
    throw polytour::CriticalError("Invalid value type for " + column_name +
                                    "column. Expected type " + stringExpectedType);
}

std::string polytour::db::utility::PostgreSqlCommandSource::buildColumnsList(const repository::Identity& repoIdentity) {
    std::string result = "(";
    for (const auto& column: repoIdentity.tableColumns) {
        if (column.second.type != TableTypes::Serial) {
            result += column.first;
            result += ", ";
        }
    }
    result.erase(result.size() - 2);
    result += ")";
    return result;
}

void polytour::db::utility::PostgreSqlCommandSource::checkFieldSetVsIdentity(
        const polytour::db::utility::FieldSet &fieldSet, const repository::Identity& repoIdentity) {
    auto tableColumns = repoIdentity.tableColumns;
    for (const auto& value: fieldSet) {
        auto tableColumnIt = tableColumns.find(value.first);
        if (tableColumnIt == tableColumns.end())
            throw polytour::CriticalError("Input fieldSet contains unexpected column name: " + value.first);
    }
}


