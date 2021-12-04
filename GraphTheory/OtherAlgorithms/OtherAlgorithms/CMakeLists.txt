add_library(Graph src/graph.cpp)
add_library(UGraph src/ugraph.cpp)
add_library(Edge src/edge.cpp)
add_library(FlowGraph src/flowgraph.cpp)

target_include_directories(Graph PUBLIC include)
target_include_directories(UGraph PUBLIC include)
target_include_directories(Edge PUBLIC include)
target_include_directories(FlowGraph PUBLIC include)
