#include "flowgraph.h"

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


void FlowGraph::addEdge(int v,const Edge& edge) {
    if(v < 0 || edge.getValue() < 0)
        throw std::length_error("v or w can not be smaller than 0");
    adj_[v].push_back(edge);

    num_vertices();
}


void FlowGraph::cleanVisited() {
    for (const auto& element : visited_) {
        visited_[element.first] = false;
    }
}

std::vector<std::vector<double>> FlowGraph::get_adjecency_matrix() {
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

void FlowGraph::num_vertices() {
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

std::size_t FlowGraph::get_num_vertices() const{
    return n_vertices_;
}

std::vector<double> FlowGraph::FordFulkersonDfs(int src, int sink) {
    

    return std::vector<double> {};
}
