cmake_minimum_required(VERSION 3.20)

project(Tuple VERSION 1.0)


set(header_files 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/List/list.hpp
)

add_library(ListClass INTERFACE)
target_sources(ListClass INTERFACE "$<BUILD_INTERFACE:${header_files}>")
target_include_directories(ListClass INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_subdirectory(tests)
endif()

add_executable(Main src/main.cpp)
set_target_properties(Main PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(Main PUBLIC ListClass)
