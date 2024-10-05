cmake_minimum_required(VERSION 3.5)

project(sorting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(SFML 2.5.1 COMPONENTS system graphics audio network REQUIRED)

configure_file(${CMAKE_SOURCE_DIR}/assets/Blop.wav
               ${CMAKE_CURRENT_BINARY_DIR}/Blop.wav
               COPYONLY)

add_library(sort_visualiser_lib
               src/constants.cc src/constants.h
               src/Beeper.cc src/Beeper.h
               src/Visualisation.h
               src/BarVisual.cc src/BarVisual.h
               src/BarDisparityVisual.cc src/BarDisparityVisual.h
               src/PieVisual.cc src/PieVisual.h
               src/StatArray.h
               src/Element.h
               src/sorting_algorithms.h
               src/viridis_palette.h
)

set(TARGET_LIBRARIES sort_visualiser_lib sfml-audio sfml-graphics sfml-network sfml-system)
add_executable(sorting src/main.cpp)
target_link_libraries(sorting PRIVATE ${TARGET_LIBRARIES})

add_subdirectory(test)
