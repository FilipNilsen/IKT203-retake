cmake_minimum_required(VERSION 3.28.6)
project(exercise3)

set(CMAKE_CXX_STANDARD 23)

add_executable(exercise3 main.cpp
        include/constants.h
        include/TQueue.h
        include/TStack.h
        include/TCoordinate.h
        include/TGrid.h
        include/TSearchAlgorithm.h
        include/IStorage.h)

add_custom_target(plantuml)
add_custom_command(PRE_BUILD TARGET plantuml COMMAND plantuml ARGS -tsvg ../main.cpp)