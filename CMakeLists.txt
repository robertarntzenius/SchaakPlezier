cmake_minimum_required(VERSION 3.22)
project(SchaakPlezier)

set(CMAKE_CXX_STANDARD 20)

# Als we heel serieus willen doen:
# add_compile_options(-Wshadow -Wconversion)
add_compile_options(-Wall -Wextra -Wpedantic -Weffc++ -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Werror -O3)

# Set the Clang compiler
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Find Clang-Tidy
# find_program(CLANG_TIDY_EXE REQUIRED NAMES "clang-tidy" "clang-tidy-10")
# if(CLANG_TIDY_EXE)
#   set(CMAKE_CXX_CLANG_TIDY
#           "${CLANG_TIDY_EXE}"
#           "-checks=bugprone-*,cppcoreguidelines-*,hicpp-*,misc-*,modernize-*,objc-*,performance-*,readability-*,-modernize-use-trailing-return-type,-readability-identifier-length")
# else()
#   message(WARNING "Clang-Tidy not found. Static analysis will not be applied.")
# endif()

add_executable(SchaakPlezier
    src/game.cpp
    src/main.cpp
)

add_library(Board SHARED
    src/movegeneration_pawn.cpp
    src/board.cpp
)

target_include_directories(SchaakPlezier PUBLIC include)
target_include_directories(Board PUBLIC include)

target_link_libraries(SchaakPlezier Board)

if(NOT BUILD_TYPE)
    message(STATUS "Build configuration defaulted to Release")
    set(BUILD_TYPE "Release")
elseif(BUILD_TYPE STREQUAL "Release")
    message(STATUS "Build configuration: ${BUILD_TYPE}")
    # Do nothing for now, maybe add optimization compiler flags later
elseif(BUILD_TYPE STREQUAL "Debug") 
    # Add this -DDEBUG which essentially has adds #define DEBUG to the files its target
    message(STATUS "Build configuration: ${BUILD_TYPE}")
    target_compile_definitions(SchaakPlezier PRIVATE -DDEBUG)
    enable_testing()
    add_subdirectory(test)
else()
    message(FATAL_ERROR "Invalid Build configuration: ${BUILD_TYPE} (Use Release or Debug)")
endif()
