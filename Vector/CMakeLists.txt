cmake_minimum_required(VERSION 3.7...3.21)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Vector VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
add_subdirectory(Vector)

add_executable(Main src/main.cpp)
add_executable(tests tests/test.cpp)

find_package(Catch2 REQUIRED)
if(Catch2_FOUND) 
    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
endif()

target_link_libraries(Main PUBLIC VectorClass)
target_link_libraries(tests PUBLIC VectorClass)


