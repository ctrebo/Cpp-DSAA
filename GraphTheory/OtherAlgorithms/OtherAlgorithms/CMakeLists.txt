add_library(Graph src/graph.cpp)
add_library(UGraph src/ugraph.cpp)
add_library(Edge src/edge.cpp)

target_include_directories(Graph PUBLIC include)
target_include_directories(UGraph PUBLIC include)
target_include_directories(Edge PUBLIC include)
