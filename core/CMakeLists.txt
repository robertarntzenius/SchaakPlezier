cmake_minimum_required(VERSION 3.22)
project(SchaakPlezier)

set(CMAKE_CXX_STANDARD 20)
get_filename_component(ROOT_DIR "${CMAKE_SOURCE_DIR}" DIRECTORY)
set(EXTERN_DIR "${ROOT_DIR}/extern")

# Als we heel serieus willen doen:

# add_compile_options(-Wshadow)
# add_compile_options(-Wconversion)
add_compile_options(-Wall -Wextra -Wpedantic -Weffc++ -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Werror -O3 -fopenmp)

# Parallel
find_package(OpenMP REQUIRED)

# Add the header only nlohmann_json submodule to the include directories
set(NLOHMANN_JSON_DIR "${EXTERN_DIR}/json")
include_directories(${NLOHMANN_JSON_DIR}/include)

# GUI stuff
find_package(Python REQUIRED COMPONENTS Interpreter Development)
include_directories(${PYTHON_INCLUDE_DIRS})

# Include the pybind11 submodule
# Specify the source directory and an explicit out-of-source build directory for pybind11
set(PYBIND11_SOURCE_DIR "${EXTERN_DIR}/pybind11")
set(PYBIND11_BINARY_DIR "${EXTERN_DIR}/pybind11/build")
add_subdirectory(${PYBIND11_SOURCE_DIR} ${PYBIND11_BINARY_DIR})
include_directories(${PYBIND11_DIR}/include)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${PYBIND11_BINARY_DIR}")

# Dont know where this came from
add_compile_options(-fPIC)

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
    set(BOARD_BUILD_TYPE_FLAGS "-DDEBUG" CACHE STRING "Compile flags added to this target")
    set(GAME_BUILD_TYPE_FLAGS "-DDEBUG" CACHE STRING "Compile flags added to this target")
elseif(BUILD_TYPE STREQUAL "Verbose")
    # Add this -DVERBOSE which essentially has adds #define VERBOSE to the files of its target
    message(STATUS "Build configuration: ${BUILD_TYPE}")
    set(BOARD_BUILD_TYPE_FLAGS "-DVERBOSE" CACHE STRING "Compile flags added to this target")
    set(GAME_BUILD_TYPE_FLAGS "-DVERBOSE" CACHE STRING "Compile flags added to this target")
else()
    message(FATAL_ERROR "Invalid Build configuration: ${BUILD_TYPE} (Use Release, Debug or Verbose)")
endif()

enable_testing()

# Profiler
if(PROFILER STREQUAL "ON")
    find_program(VALGRIND_EXECUTABLE valgrind)
    if(VALGRIND_EXECUTABLE)
        add_compile_options(-g)
        message(STATUS "Compiling for profiler with debugging symbols")
    else()
        message(WARNING "Valgrind not found. Profiling not supported.")
    endif()
endif()

add_executable(SchaakPlezier
    main.cpp
)

target_compile_definitions(SchaakPlezier PRIVATE ${BUILD_TYPE_FLAGS})

add_subdirectory(board)
add_subdirectory(game)
add_subdirectory(api)
