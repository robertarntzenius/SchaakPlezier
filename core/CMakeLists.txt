cmake_minimum_required(VERSION 3.22)
project(SchaakPlezier)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# External dependencies
get_filename_component(ROOT_DIR "${CMAKE_SOURCE_DIR}" DIRECTORY)
set(EXTERN_DIR "${ROOT_DIR}/extern")

set(CMAKE_INSTALL_BINDIR "bin")
set(CMAKE_INSTALL_LIBDIR "lib")

add_compile_options(-Wall -Wextra -Wpedantic -Weffc++ -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Werror -O3 -fopenmp -fPIC)

# Parallel
find_package(OpenMP REQUIRED)

# Add the header only nlohmann_json submodule to the include directories
set(NLOHMANN_JSON_DIR "${EXTERN_DIR}/json")
include_directories(${NLOHMANN_JSON_DIR}/include)

# Build configuration
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

set_target_properties(SchaakPlezier PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"  # Same as above
    BUILD_WITH_INSTALL_RPATH TRUE  # Ensure RPATH is set during build
)

target_compile_definitions(SchaakPlezier PRIVATE ${BUILD_TYPE_FLAGS})

add_subdirectory(board)
add_subdirectory(game)
add_subdirectory(api)

# Set RPATH
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

install(TARGETS SchaakPlezier
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}        # For executables (Windows/Linux)
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}        # For shared libraries (Linux/Unix)
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}        # For static libraries
)
