#ifndef FLOWGRAPH_H
#define FLOWGRAPH_H

#include "edge.h"

#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <cstddef>
#include <array>
#include <stack>
#include <utility>

// In this Graph the weight of a edge acts
// as its capacity
class FlowGraph {
private:
    std::map<int, bool> visited_ {};
    std::map<int, std::vector<Edge>> adj_ {};
    std::size_t n_vertices_{};
    
    // Return number of vertices in graph
    void num_vertices();

    // Function to set all elements in "visited_" to false
    // so it can be used again
    void cleanVisited();

    // Function that returns adjecency matrix. Distance from node to
    // itself is 0 and not reachable nodes are markes as infinity
    std::vector<std::vector<double>> get_adjecency_matrix();
    

public:
    // Function to add an edge to graph
    void addEdge(int v,const Edge& edge);
    std::size_t get_num_vertices() const; 
    
    std::vector<double> FordFulkersonDfs(int src, int sink);
};

#endif // FLOWGRAPH_H
