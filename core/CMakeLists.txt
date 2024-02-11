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
#           "-checks=bugprone-*,cppcoreguidelines-*,hicpp-*,misc-*,modernize-*,objc-*,performance-*,readability-*,-modernize-use-trailing-return-type,-readability-identifier-length,-cppcoreguidelines-pro-bounds-constant-array-index")
# else()
#   message(WARNING "Clang-Tidy not found. Static analysis will not be applied.")
# endif()

if(NOT BUILD_TYPE)
    message(STATUS "Build configuration defaulted to Release")
    set(BUILD_TYPE "Release")
elseif(BUILD_TYPE STREQUAL "Release")
    message(STATUS "Build configuration: ${BUILD_TYPE}")
    # Do nothing for now, maybe add optimization compiler flags later
elseif(BUILD_TYPE STREQUAL "Debug")
    # Add this -DDEBUG which essentially has adds #define DEBUG to the files of its target
    message(STATUS "Build configuration: ${BUILD_TYPE}")
    set(BUILD_TYPE_FLAGS "-DDEBUG" CACHE STRING "Compile flags added to this target")
    enable_testing()
elseif(BUILD_TYPE STREQUAL "Verbose")
    # Add this -DVERBOSE which essentially has adds #define VERBOSE to the files of its target
    message(STATUS "Build configuration: ${BUILD_TYPE}")
    set(BUILD_TYPE_FLAGS "-DVERBOSE" CACHE STRING "Compile flags added to this target")
    enable_testing()
else()
    message(FATAL_ERROR "Invalid Build configuration: ${BUILD_TYPE} (Use Release, Debug or Verbose)")
endif()


add_executable(SchaakPlezier
    main.cpp
)

target_compile_definitions(SchaakPlezier PRIVATE ${BUILD_TYPE_FLAGS})

add_subdirectory(board)
add_subdirectory(game)
