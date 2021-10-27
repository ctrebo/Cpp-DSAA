#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"

#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <cstddef>
#include <array>
#include <stack>
#include <utility>




// Graph class represents a directed graph
// using adjecency list representation
class Graph {
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

    // Functions and subfunctions for tarjans
    void tarjansDfs(std::size_t at,int& scc_count, int& id, std::stack<int>& stack, std::vector<int>& ids, std::vector<bool>& on_stack, std::vector<int>& low);    

    

public:
    // Function to add an edge to graph
    void addEdge(int v,const Edge& edge);

    // Algorithm to find strongly connected components
    std::vector<int> tarjans();


    std::size_t get_num_vertices();

};

#endif // GRAPH_H
