cmake_minimum_required(VERSION 3.21)
project(db)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} STATIC
        inc/public/IDatabaseAdapter.h
        inc/public/PostgreSqlAdapter.h
        src/PostgreSqlAdapter.cpp
        inc/public/IDatabaseExecResult.h
        inc/public/utility/FieldSet.h
        inc/private/pqxx/converters/ResultConverter.h
        inc/public/utility/ICommandSource.h
        src/utility/PostgreSqlCommandSource.cpp
        inc/public/utility/PostgreSqlCommandSource.h
        inc/public/repositories/Identity.h
        inc/public/utility/BaseDefines.h
        src/utility/TableAbstractValue.cpp
        inc/public/utility/TableAbstractValue.h
        inc/public/IConnection.h src/SqlConnection.cpp
        inc/public/SqlConnection.h
        inc/public/repositories/IRepository.h
        inc/public/repositories/IRepositoryInitializer.h
        inc/public/repositories/Repository.h
        src/repositories/UserRepository.cpp
        inc/public/repositories/UserRepository.h
        src/repositories/TournamentRepository.cpp
        inc/public/repositories/TournamentRepository.h
        src/repositories/MatchRepository.cpp
        inc/public/repositories/MatchRepository.h
        src/repositories/RepositoryFactory.cpp
        inc/public/repositories/RepositoryFactory.h
        inc/public/repositories/IRepositoryFactory.h
        src/repositories/TournamentParticipantsRepository.cpp
        inc/public/repositories/TournamentParticipantsRepository.h
        inc/public/repositories/roles/IRole.h
        src/repositories/roles/GuestRole.cpp
        inc/public/repositories/roles/GuestRole.h
        src/repositories/roles/TournamentParticipantRole.cpp
        inc/public/repositories/roles/TournamentParticipantRole.h
        src/repositories/roles/TournamentLeaderRole.cpp
        inc/public/repositories/roles/TournamentLeaderRole.h
        inc/public/ActiveRoleSingleton.h
        inc/public/repositories/IRepositoryFactoryCreator.h
        src/repositories/roles/RolesConverter.cpp
        inc/public/repositories/roles/RolesConverter.h
        src/repositories/RepositoryFactoryCreator.cpp
        inc/public/repositories/RepositoryFactoryCreator.h
        inc/public/repositories/roles/CommonUserRole.h
        src/repositories/roles/CommonUserRole.cpp
        src/utility/JsonConfiguration.cpp
        inc/public/utility/JsonConfiguration.h
        src/mysql/MySqlAdapter.cpp
        inc/public/mysql/MySqlAdapter.h
        inc/private/cppconn/converters/ResultConverter.h src/utility/MySqlCommandSource.cpp inc/public/utility/MySqlCommandSource.h)

target_link_libraries(${PROJECT_NAME} transport)
target_link_libraries(${PROJECT_NAME} pqxx)
target_link_libraries(${PROJECT_NAME} jsoncpp_lib)
target_link_libraries(${PROJECT_NAME} mysqlcppconn)
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PUBLIC ../transport)

add_library(${PROJECT_NAME}-tests-mocks
        tests/mocks/MockDatabaseAdapter.h
        tests/mocks/MockDatabaseExecResult.h
        tests/mocks/MockCommandSource.h
        tests/mocks/MockRepositoryFactoryCreator.h
        tests/mocks/MockRepositoryFactory.h
        tests/mocks/MockRepository.h)

target_link_libraries(${PROJECT_NAME}-tests-mocks ${PROJECT_NAME} gtest gmock gtest_main transport)
target_include_directories(${PROJECT_NAME}-tests-mocks PUBLIC ../transport)

add_executable(${PROJECT_NAME}-tests
        tests/TestSqlConnection.cpp
        tests/TestPostgreSqlCommandSource.cpp
        tests/TestTableAbstractValue.cpp
        tests/NotMandatoryFieldTests.cpp
        tests/FieldSetTests.cpp tests/integration/integration_tests.cpp)

target_link_libraries(${PROJECT_NAME}-tests ${PROJECT_NAME} ${PROJECT_NAME}-tests-mocks gtest gmock gtest_main transport)
target_include_directories(${PROJECT_NAME}-tests PUBLIC ../transport ../db/tests/mocks)

add_subdirectory(dependencies/libpqxxx)
add_subdirectory(dependencies/gtest)
add_subdirectory(dependencies/jsoncpp)

