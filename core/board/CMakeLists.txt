add_library(Board SHARED
    src/movegeneration_pawn.cpp
    src/movegeneration_knight.cpp
    src/movegeneration_sliders.cpp 
    src/movegeneration_king.cpp
    src/board.cpp
)

target_compile_definitions(Board PRIVATE ${BOARD_BUILD_TYPE_FLAGS})

target_include_directories(Board PUBLIC include) 

target_link_libraries(SchaakPlezier Board)

add_subdirectory(test)
