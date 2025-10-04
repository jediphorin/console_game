cmake_minimum_required(VERSION 3.31.6 FATAL_ERROR)
project(AutoBattler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим GTest
find_package(GTest REQUIRED)

add_library(combat STATIC 
    src/combat/Character.cpp
    src/combat/Monster.cpp
)
target_include_directories(combat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(game_logic STATIC src/game/Game.cpp)
target_include_directories(game_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(game_logic combat)

add_executable(auto_battler src/main.cpp)
target_link_libraries(auto_battler game_logic combat)

# Тесты
add_executable(character_tests test/character_test.cpp)
target_link_libraries(character_tests GTest::gtest GTest::gtest_main combat game_logic)
target_include_directories(character_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(monster_tests test/monster_test.cpp)
target_link_libraries(monster_tests GTest::gtest GTest::gtest_main combat game_logic)
target_include_directories(monster_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(combat_tests test/combat_system_test.cpp)
target_link_libraries(combat_tests GTest::gtest GTest::gtest_main combat game_logic)
target_include_directories(combat_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(barbara_rage_test test/barbarian_rage_test.cpp)
target_link_libraries(barbara_rage_test GTest::gtest GTest::gtest_main combat game_logic)
target_include_directories(barbara_rage_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(poison_test test/poison_mechanic_test.cpp)
target_link_libraries(poison_test GTest::gtest GTest::gtest_main combat game_logic)
target_include_directories(poison_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(game_tests test/game_logic_test.cpp)
target_link_libraries(game_tests GTest::gtest GTest::gtest_main combat game_logic)
target_include_directories(game_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Все тесты вместе
add_executable(all_tests 
    test/character_test.cpp
    test/monster_test.cpp  
    test/combat_system_test.cpp
    test/barbarian_rage_test.cpp
    test/poison_mechanic_test.cpp
    test/game_logic_test.cpp    
)
target_link_libraries(all_tests GTest::gtest GTest::gtest_main combat game_logic)
target_include_directories(all_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)