cmake_minimum_required(VERSION 3.20)

project(HashTable VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/HashTable/hashtable.hpp
)

add_library(HashTable INTERFACE)
target_sources(HashTable INTERFACE "$<BUILD_INTERFACE:${header_files}>")
target_include_directories(HashTable INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_subdirectory(tests)
endif()

add_executable(Main src/main.cpp)

target_link_libraries(Main PUBLIC HashTable)
