//
// Created by denis on 27.08.2022.
//

#ifndef COURSEWORKDB_JSONCONFIGURATION_H
#define COURSEWORKDB_JSONCONFIGURATION_H

#include <string>

namespace polytour::db::utility {

    class JsonConfiguration;

    class DbConfiguration {
    public:
        enum class Sqls {
            Postgres,
            MongoDb
        };

        std::string table_users_name;
        std::string table_matches_name;
        std::string table_tournaments_name;
        std::string table_tournament_participants_name;
        std::string db_name;
        Sqls used_sql;

    private:

        friend JsonConfiguration;
        void parse(const std::string& pathToConfFile);

    };

    class JsonConfiguration {
    public:
        static void parse(const std::string& pathToConfFile);

        static DbConfiguration* getInstance();
    };
}


#endif //COURSEWORKDB_JSONCONFIGURATION_H
