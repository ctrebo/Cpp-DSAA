cmake_minimum_required(VERSION 3.20)

project(Tuple VERSION 1.0)


add_subdirectory(Tuple)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_subdirectory(tests)
endif()

add_executable(Main src/main.cpp)
set_target_properties(Main PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(Main PUBLIC TupleClass)
