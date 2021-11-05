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

    // Functions and subfunctions for topsort
    int dfstopsort(int i, int at, std::vector<int>& ordering);

    // Functions and subfunctions for optiamal Path with dijkstras algo
    std::pair<std::vector<double>, std::vector<double>> dijkstrasOptimalPath(int start);

public:
    // Function to add an edge to graph
    void addEdge(int v,const Edge& edge);

    std::vector<int> topsort();
    
    // Used double vectors because double has infinity property
    std::vector<double> dagShortestPath(int start);
    std::vector<double> dijkstras(int start);
    std::vector<double> mainDijkstrasOptimalPath(int start, int end);
    std::vector<double> bellmanFord(int start);
    std::vector<std::vector<double>> floydWarshall();


    std::size_t get_num_vertices();

};

#endif // GRAPH_H
