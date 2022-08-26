cmake_minimum_required(VERSION 3.21)
project(ui)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}_lib STATIC
        ../transport/trash.cpp #TODO Remove this shit
        inc/windows/IAuthorizationWindow.h
        inc/windows/IWindowsFactory.h
        inc/windows/IWindow.h
        inc/ICoordinator.h
        inc/windows/IMainMenuWindow.h
        inc/windows/ISingUpWindow.h
        inc/windows/IUpdateUserWindow.h
        inc/windows/ITournamentCreationWindow.h
        inc/windows/ITournamentWindow.h)
target_include_directories(${PROJECT_NAME}_lib PUBLIC inc)

add_library(${PROJECT_NAME}_lib_cdk STATIC
        src/commonUser/CommonUserWindowsFactory.cpp
        inc/windows/cdk/commonUser/CommonUserWindowsFactory.h
        src/windows/cdk/AuthorizationWindow.cpp
        inc/windows/cdk/AuthorizationWindow.h
        src/CdkCoordinator.cpp
        inc/CdkCoordinator.h
        src/windows/cdk/MainMenuWindow.cpp
        inc/windows/cdk/MainMenuWindow.h
        src/windows/cdk/CdkWindowsFactory.cpp
        inc/windows/cdk/CdkWindowsFactory.h
        src/windows/cdk/SignUpWindow.cpp
        inc/windows/cdk/SignUpWindow.h
        src/windows/cdk/UpdateUserWindow.cpp
        inc/windows/cdk/UpdateUserWindow.h src/windows/cdk/TournamentCreationWindow.cpp inc/windows/cdk/TournamentCreationWindow.h src/windows/cdk/GuestTournamentWindow.cpp inc/windows/cdk/GuestTournamentWindow.h src/windows/cdk/ParticipantTournamentWindow.cpp inc/windows/cdk/ParticipantTournamentWindow.h src/windows/cdk/LeaderWaitTournamentWindow.cpp inc/windows/cdk/LeaderWaitTournamentWindow.h src/windows/cdk/LeaderStartedTournamentWindow.cpp inc/windows/cdk/LeaderStartedTournamentWindow.h)

target_include_directories(${PROJECT_NAME}_lib_cdk PUBLIC inc)
target_link_libraries (${PROJECT_NAME}_lib_cdk PUBLIC cdk curses ${PROJECT_NAME}_lib transport bl)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib ${PROJECT_NAME}_lib_cdk)



