#ifndef UGRAPH_H
#define UGRAPH_H

#include "edge.h"

#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <cstddef>
#include <array>
#include <stack>
#include <utility>

struct Triplet;
class UGraph {
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

    // Functions and subfunctions for Prims MST
    void addEdgesPrims(int node_index, std::priority_queue<Triplet>& pq);


public:
    // Function to add an edge to graph
    void addEdge(int v,const Edge& edge);
    std::size_t get_num_vertices();

    std::pair<int, std::vector<Triplet>> prims(int start=0);

};

struct Triplet
{
    int edge_from_;
    int edge_to_;
    int edge_weight_;

    Triplet() = default;
    
    Triplet(int edge_from, int edge_to, int edge_weight) : edge_from_{ edge_from }, edge_to_{ edge_to }, edge_weight_{ edge_weight }
    {
    }

    bool operator<(const struct Triplet& other) const {
        //Your priority logic goes here
        return edge_weight_ > other.edge_weight_;
    }
};
#endif // UGRAPH_H
