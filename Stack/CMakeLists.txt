cmake_minimum_required(VERSION 3.20)
project(Stack VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Stack)

add_executable(Main src/main.cpp)

target_link_libraries(Main PUBLIC Stack)
target_link_libraries(Main PUBLIC twoStack)
