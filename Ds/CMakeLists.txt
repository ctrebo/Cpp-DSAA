cmake_minimum_required(VERSION 3.20)

project(Ds VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Ds/hashtable.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Ds/hashtablewllist.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Ds/list.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Ds/binarysearchtree.hpp
)

add_library(Ds INTERFACE)
target_sources(Ds INTERFACE "$<BUILD_INTERFACE:${header_files}>")
target_include_directories(Ds INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_subdirectory(tests)
endif()

add_executable(Main src/main.cpp)

target_link_libraries(Main PUBLIC Ds)
