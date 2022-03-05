add_library(twoStack INTERFACE)
add_library(Stack src/stack.cpp)

# link everything for 'twoStack' target
target_sources(twoStack INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/twostack.hpp>")
target_include_directories(twoStack INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)

# link everything for 'Stack' target
target_include_directories(Stack PUBLIC include/)


