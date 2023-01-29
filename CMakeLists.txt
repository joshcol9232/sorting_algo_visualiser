cmake_minimum_required(VERSION 3.5)

project(sorting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5.1 COMPONENTS system graphics audio network REQUIRED)

configure_file(${CMAKE_SOURCE_DIR}/assets/Blop.wav
               ${CMAKE_CURRENT_BINARY_DIR}/Blop.wav
               COPYONLY)

add_executable(sorting src/main.cpp
               src/sortalgo.cpp src/sortalgo.h
               src/sortarray.cpp src/sortarray.h
               src/constants.h
               # Sorting algos
               src/sorting_algos/bubble.cpp src/sorting_algos/bubble.h
               src/sorting_algos/bogo.cpp src/sorting_algos/bogo.h)

target_link_libraries(sorting PRIVATE sfml-audio sfml-graphics sfml-network sfml-system)

