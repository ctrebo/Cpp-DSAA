add_library(Node src/node.cpp)
add_library(LinkedList src/linkedlist.cpp)

target_include_directories(Node PUBLIC include)
target_include_directories(LinkedList PUBLIC include)
