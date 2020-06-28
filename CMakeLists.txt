cmake_minimum_required(VERSION 3.5)

project(BullsAndCows LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(BullsAndCows main.cpp)
