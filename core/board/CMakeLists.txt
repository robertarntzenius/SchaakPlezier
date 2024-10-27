add_library(Board SHARED
    src/movegeneration_pawn.cpp
    src/movegeneration_knight.cpp
    src/movegeneration_sliders.cpp
    src/movegeneration_king.cpp
    src/board.cpp
)
set_target_properties(Board PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"  # Path to find shared libraries at runtime
    BUILD_WITH_INSTALL_RPATH TRUE  # Ensure RPATH is set during build
)
target_compile_definitions(Board PRIVATE ${BOARD_BUILD_TYPE_FLAGS})

target_include_directories(Board PUBLIC include)

target_link_libraries(SchaakPlezier Board)

add_subdirectory(test)

install(TARGETS Board
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}  # Install shared library
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}   # Install static library (if applicable)
)
