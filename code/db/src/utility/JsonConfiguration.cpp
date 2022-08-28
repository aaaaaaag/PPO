//
// Created by denis on 27.08.2022.
//

#include "public/utility/JsonConfiguration.h"
#include "json/json.h"
#include <fstream>
#include <sstream>
#include "memory"

void polytour::db::utility::DbConfiguration::parse(const std::string &pathToConfFile) {
    auto ss = std::ostringstream{};
    std::ifstream file(pathToConfFile);
    ss << file.rdbuf();
    auto fileContents = ss.str();
    Json::Value root;
    Json::Reader reader;
    reader.parse(fileContents, root);
    table_users_name = root["users_table"]["name"].asString();
    table_tournaments_name = root["tournaments_table"]["name"].asString();
    table_matches_name = root["matches_table"]["name"].asString();
    table_tournament_participants_name = root["tournament_participants_table"]["name"].asString();
    db_name = root["db_name"].asString();
    auto usedSql = root["used_sql"].asString();
    if (usedSql == "postgres")
        used_sql = Sqls::Postgres;
    else if (usedSql == "mysql")
        used_sql = Sqls::MySql;
    else
        throw std::logic_error("Configuration file contains invalid used sql name");
}

static std::unique_ptr<polytour::db::utility::DbConfiguration> _pInstance = std::make_unique<decltype(_pInstance)::element_type>();

polytour::db::utility::DbConfiguration *polytour::db::utility::JsonConfiguration::getInstance() {
    return _pInstance.get();
}

void polytour::db::utility::JsonConfiguration::parse(const std::string &pathToConfFile) {
    _pInstance->parse(pathToConfFile);
}
