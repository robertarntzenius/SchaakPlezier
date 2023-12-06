cmake_minimum_required(VERSION 3.16)
project(SchaakPlezier)

set(CMAKE_CXX_STANDARD 14)
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
