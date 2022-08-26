cmake_minimum_required(VERSION 3.21)
project(bl)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} STATIC
        inc/transactions/IUserTransactionFactory.h
        inc/transactions/UserTransactionFactory.h
        src/transactions/UserTransactionFactory.cpp
        inc/transactions/ITournamentTransactionFactory.h
        src/transactions/TournamentTransactionFactory.cpp
        inc/transactions/TournamentTransactionFactory.h
        inc/transactions/IMatchTransactionFactory.h
        inc/AuthUserSingleton.h
        inc/facade/IMainFacade.h
        inc/facade/IUserFacade.h
        inc/facade/ITournamentFacade.h
        inc/facade/IMatchFacade.h
        src/transactions/MatchTransactionFactory.cpp
        inc/transactions/MatchTransactionFactory.h
        src/facade/UserFacade.cpp
        inc/facade/UserFacade.h
        src/facade/TournamentFacade.cpp
        inc/facade/TournamentFacade.h
        src/facade/MatchFacade.cpp
        inc/facade/MatchFacade.h
        src/facade/MainFacade.cpp
        inc/facade/MainFacade.h
        inc/facade/IFacadeErrorProcessor.h
        src/ErrorObj.cpp
        inc/ErrorObj.h
        inc/CurrentRoleSingleton.h)

target_link_libraries(${PROJECT_NAME} transport db)
target_include_directories(${PROJECT_NAME} PUBLIC inc)

add_executable(${PROJECT_NAME}-tests tests/UserTransactionFactoryTests.cpp tests/MatchTransactionFactoryTests.cpp tests/TournamentTransactionFactoryTests.cpp)

target_link_libraries(${PROJECT_NAME}-tests ${PROJECT_NAME} transport gmock gtest gtest_main db-tests-mocks)
target_include_directories(${PROJECT_NAME}-tests PUBLIC ../transport ../db/tests/mocks)