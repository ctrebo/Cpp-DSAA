#include "ugraph.h"

#include <iostream>
#include <algorithm>
#include <exception>
#include <limits>
#include <algorithm>
#include <utility>
#include <queue>
#include <cstddef>
#include <array>
#include <stack>
#include <tuple>

void UGraph::addEdge(int v,const Edge& edge) {
    if(v < 0 || edge.getValue() < 0)
        throw std::length_error("v or w can not be smaller than 0");
    adj_[v].push_back(edge);
    adj_[edge.getValue()].push_back(Edge{v, edge.getWeight()});

    num_vertices();
}


void UGraph::cleanVisited() {
    for (const auto& element : visited_) {
        visited_[element.first] = false;
    }
}

std::vector<std::vector<double>> UGraph::get_adjecency_matrix() {
    std::vector<std::vector<double>> m(n_vertices_ , std::vector<double> (n_vertices_, std::numeric_limits<double>::infinity()));
    for(const auto& element: adj_) {
        for(const auto& element1: element.second) {
            m[element.first][element1.getValue()] = element1.getWeight();
        }
    }
    // Distance from note to itself is 0
    for(std::size_t i {0}; i < n_vertices_; ++i) {
        m[i][i] = 0;
    }

    return m;
}


void UGraph::num_vertices() {
    n_vertices_ = 0;
    
    for(const auto& element : adj_) {
        if(!visited_[element.first]) {
            visited_[element.first] = true;
            ++n_vertices_;
        }
        for(const auto element1 : element.second) {
            if(!visited_[element1.getValue()]) {
                visited_[element1.getValue()] = true;
                ++n_vertices_;
            }
        }
    }
    cleanVisited();
    
}

std::size_t UGraph::get_num_vertices() {
    return n_vertices_;
}


void UGraph::addEdgesPrims(int node_index, std::priority_queue<Triplet>& pq ) {
    visited_[node_index] = true;
    for(const auto& element :adj_[node_index]) {
        if(!visited_[element.getValue()]) {
            pq.push(Triplet{node_index, element.getValue(), element.getWeight()});
        }
    }
}

std::pair<int, std::vector<Triplet>> UGraph::prims(int start) {
    std::size_t m {n_vertices_ - 1}; // number of edges in MST
    int edge_count{0};
    int mst_cost {0};
    std::priority_queue<Triplet> pq {};
    std::vector<Triplet> edges_mst(m, Triplet{});
    addEdgesPrims(start, pq);

    while(!pq.empty() && edge_count != m) {
        Triplet edge {pq.top()};
        pq.pop();
        int node_index = edge.edge_to_; 
        if(visited_[node_index]) continue;

        edges_mst[edge_count++] = edge;
        mst_cost = mst_cost + edge.edge_weight_;

        addEdgesPrims(node_index, pq);
    }
    cleanVisited();
    if(edge_count!=m) return std::make_pair(0, std::vector<Triplet>{});

    return std::make_pair(mst_cost, edges_mst);
}

