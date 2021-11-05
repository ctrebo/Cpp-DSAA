add_library(Graph src/graph.cpp)
add_library(Edge src/edge.cpp)

target_include_directories(Graph PUBLIC include)
target_include_directories(Edge PUBLIC include)
