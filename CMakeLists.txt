cmake_minimum_required(VERSION 3.26)
project(SchaakPlezier)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(SchaakPlezier
        include/board.h
        include/game.h
        include/piece.h
        src/board.cpp
        src/game.cpp
        src/main.cpp)
