cmake_minimum_required(VERSION 3.16)
project(SchaakPlezier)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(SchaakPlezier
        include/board.h
        include/game.h
        include/piece.h
        include/bitboard.h

        src/board.cpp
        src/bitboard.cpp
        src/game.cpp
        src/main.cpp)
