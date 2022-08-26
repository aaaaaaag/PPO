cmake_minimum_required(VERSION 3.21)
project(CourseWorkDB)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(db)
add_subdirectory(transport)
add_subdirectory(bl)
add_subdirectory(ui)

target_link_libraries(${PROJECT_NAME} PUBLIC db bl transport ui_lib ui_lib_cdk)
