set(GAME_TARGET "game")
set(GAME_TARGET "game" PARENT_SCOPE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${GAME_TARGET} "chesspiece.h" "chesspiece.cpp" "chessboard.h" "chessboard.cpp")

find_package(SFML REQUIRED system window graphics)

if (${SFML_FOUND})
    message("sfml was found!!")
else()
    message("sfml was not found!")
endif()

include_directories(${SFML_INCLUDE_DIR})

target_link_libraries(${GAME_TARGET} sfml-graphics)
