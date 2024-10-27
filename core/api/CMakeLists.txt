find_package(Python REQUIRED COMPONENTS Interpreter Development)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(pybind11 REQUIRED)

pybind11_add_module(wrappers
    src/wrappers.cpp
)

set_target_properties(wrappers PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"  # Path to find shared libraries at runtime
    BUILD_WITH_INSTALL_RPATH TRUE  # Ensure RPATH is set during build
)

target_link_libraries(wrappers PRIVATE Board pybind11::module)
target_link_libraries(wrappers PRIVATE Game pybind11::module)

target_compile_definitions(wrappers PRIVATE ${BUILD_TYPE_FLAGS})

target_include_directories(wrappers PUBLIC include)

add_subdirectory(test)

install(TARGETS wrappers
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}  # Install shared library
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}   # Install static library (if applicable)
)
