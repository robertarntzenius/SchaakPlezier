add_library(Game SHARED
    src/game.cpp
    src/humanplayer.cpp
    src/playerfactory.cpp
)

target_compile_definitions(Game PRIVATE ${BUILD_TYPE_FLAGS})

target_include_directories(Game PUBLIC include)
target_link_libraries(Game PUBLIC Board)

target_link_libraries(SchaakPlezier Game)

add_subdirectory(test)
