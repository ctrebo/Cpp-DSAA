add_library(Node src/node.cpp)
add_library(lList src/lList.cpp)

target_include_directories(Node PUBLIC include)
target_include_directories(lList PUBLIC include)
