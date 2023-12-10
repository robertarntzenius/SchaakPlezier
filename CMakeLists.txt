cmake_minimum_required(VERSION 3.16)
project(SchaakPlezier)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic -Weffc++ -O3)

add_executable(SchaakPlezier
        include/board.h
        include/game.h
        include/definitions.h
        include/bitboard.h
        include/log.h

        src/board.cpp
        src/bitboard.cpp
        src/game.cpp
        src/log.cpp
        src/main.cpp
        src/maskgeneration.cpp
)

target_include_directories(SchaakPlezier PUBLIC include)

if(NOT BUILD_TYPE)
    message(STATUS "Build configuration defaulted to Release")
    set(BUILD_TYPE "Release")
elseif(BUILD_TYPE STREQUAL "Release")
    message(STATUS "Build configuration: ${BUILD_TYPE}")
    # Do nothing for now, maybe add optimization compiler flags later
    # target_compile_definitions(SchaakPlezier PRIVATE -O3)
elseif(BUILD_TYPE STREQUAL "Debug") 
    # Add this -DDEBUG which essentially has adds #define DEBUG to the files its target
    message(STATUS "Build configuration: ${BUILD_TYPE}")
    target_compile_definitions(SchaakPlezier PRIVATE -DDEBUG)
else()
    message(FATAL_ERROR "Invalid Build configuration: ${BUILD_TYPE} (Use Release or Debug)")
endif()
