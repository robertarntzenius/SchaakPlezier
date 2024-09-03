find_package(Python REQUIRED COMPONENTS Interpreter Development)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(pybind11 REQUIRED)

pybind11_add_module(wrappers
    src/wrappers.cpp
)
target_link_libraries(wrappers PRIVATE Board pybind11::module)
target_link_libraries(wrappers PRIVATE Game pybind11::module)

target_compile_definitions(wrappers PRIVATE ${BUILD_TYPE_FLAGS})

target_include_directories(wrappers PUBLIC include)

add_subdirectory(test)
