cmake_minimum_required(VERSION 3.7...3.21)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(LinkedListClass VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(LinkedListClass)

add_executable(Main src/main.cpp)

target_link_libraries(Main PUBLIC lList)
target_link_libraries(Main PUBLIC Node)

target_include_directories(Main PUBLIC "${PROJECT_SOURCE_DIR}/LinkedListClass")
