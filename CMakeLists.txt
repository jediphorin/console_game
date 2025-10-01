cmake_minimum_required(VERSION 3.31.6 FATAL_ERROR)
project(AutoBattler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(combat STATIC 
    src/combat/Character.cpp
    src/combat/Monster.cpp
    # src/combat/Weapon.cpp
)
target_include_directories(combat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(game_logic STATIC src/game/Game.cpp)
target_include_directories(game_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(game_logic combat)

add_executable(auto_battler src/main.cpp)
target_link_libraries(auto_battler game_logic combat)