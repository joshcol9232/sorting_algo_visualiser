cmake_minimum_required(VERSION 3.5)

project(sorting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include_directories("")

find_package(SFML 2.5.1 COMPONENTS system graphics audio network REQUIRED)

add_executable(sorting main.cpp
               constants.h)

target_link_libraries(sorting PRIVATE sfml-audio sfml-graphics sfml-network sfml-system)

