cmake_minimum_required(VERSION 3.21)
project(transport)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        trash.cpp
        IIdentifiable.h
        Match.h
        NotMandatoryField.h Tournament.h User.h TournamentParticipant.h CriticalError.h NotCriticalError.h Roles.h)