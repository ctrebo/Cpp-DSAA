add_library(Queue INTERFACE)

target_sources(Queue INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/queue.hpp>")

target_include_directories(Queue INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)


