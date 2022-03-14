cmake_minimum_required(VERSION 3.20)

project(Tuple VERSION 1.0)


set(header_files 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Tuple/tuple.hpp
)

add_library(TupleClass INTERFACE)
target_sources(TupleClass INTERFACE "$<BUILD_INTERFACE:${header_files}>")
target_include_directories(TupleClass INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_subdirectory(tests)
endif()

add_executable(Main src/main.cpp)
set_target_properties(Main PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(Main PUBLIC TupleClass)
