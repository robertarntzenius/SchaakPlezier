add_library(Game SHARED
    src/game.cpp
    src/humanplayer.cpp
    src/playerfactory.cpp
)

set_target_properties(Game PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"  # Path to find shared libraries at runtime
    BUILD_WITH_INSTALL_RPATH TRUE  # Ensure RPATH is set during build
)

target_compile_definitions(Game PRIVATE ${GAME_BUILD_TYPE_FLAGS})

target_include_directories(Game PRIVATE extern/json/single_include)

target_include_directories(Game PUBLIC include)
target_link_libraries(Game PUBLIC Board)
target_link_libraries(Game PRIVATE OpenMP::OpenMP_CXX)

target_link_libraries(SchaakPlezier Game)

add_subdirectory(test)

install(TARGETS Game
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}  # Install shared library
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}   # Install static library (if applicable)
)
