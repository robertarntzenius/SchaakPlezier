
file(GLOB TEST_SOURCES "test*.cpp")

# Enable testing
enable_testing()

foreach(TEST_SOURCE ${TEST_SOURCES})
    # Extract the test name (excluding the .cc extension)
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Create a test executable for each test source file
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    # Link any libraries needed for the test
    target_link_libraries(${TEST_NAME} Board)

    # Add the test
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    # Exclude Clang-Tidy checks for the test files
    set_target_properties(${TEST_NAME} PROPERTIES
            CXX_CLANG_TIDY "")
    
    target_compile_definitions(${TEST_NAME} PRIVATE ${BUILD_TYPE_FLAGS})
    
endforeach()