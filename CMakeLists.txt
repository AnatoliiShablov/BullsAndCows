cmake_minimum_required(VERSION 3.5)

project(BullsAndCows LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BullsAndCows
    main.cpp
    include/bulls_cows.hpp src/bulls_cows.cpp
    include/player.hpp src/player.cpp
    include/AI_hard_player.hpp src/AI_hard_player.cpp
)

target_include_directories(BullsAndCows PRIVATE include)
