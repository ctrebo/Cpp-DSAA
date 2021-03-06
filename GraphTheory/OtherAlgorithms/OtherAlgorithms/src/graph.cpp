#include "graph.h"

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


void Graph::addEdge(int v,const Edge& edge) {
    if(v < 0 || edge.getValue() < 0)
        throw std::length_error("v or w can not be smaller than 0");
    adj_[v].push_back(edge);

    num_vertices();
}


void Graph::cleanVisited() {
    for (const auto& element : visited_) {
        visited_[element.first] = false;
    }
}

std::vector<std::vector<double>> Graph::get_adjecency_matrix() {
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

void Graph::num_vertices() {
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

std::size_t Graph::get_num_vertices() {
    return n_vertices_;
}


void Graph::tarjansDfs(std::size_t at,int& scc_count,int& id, std::stack<int>& stack, std::vector<int>& ids, std::vector<bool>& on_stack, std::vector<int>& low) {
    static const int unvisited {-1};
    stack.push(at);
    on_stack[at] = true;
    ids[at] = low[at] = id++;

    // Visit all neighbors and min low-link on callback
    for(const auto& element : adj_[at]) {
        if(ids[element.getValue()] == unvisited)
            tarjansDfs(element.getValue(), scc_count, id, stack, ids, on_stack, low);
        if(on_stack[element.getValue()])
            low[at] = std::min(low[at], low[element.getValue()]) ;
        
    }

    // If at is the beginning of a completed strongly connected component
    // empyt Stack until we are back at beginning of the SCC
    if(ids[at] == low[at]) {
        for(int node {stack.top()}; !stack.empty(); node = stack.top()) {
            stack.pop();
            on_stack[node] = false;
            low[node] = ids[at];
            if(node==at) break;
        }
        ++scc_count;
    }
}

std::vector<int> Graph::tarjans() {
    const int unvisited {-1};
    int id {0};
    int scc_count {0};
    std::vector<int> ids (n_vertices_, unvisited);
    std::vector<int> low (n_vertices_, 0);
    std::vector<bool> on_stack (n_vertices_, false);
    std::stack<int> stack {};

    for(std::size_t i {0}; i < n_vertices_; ++i) ids[i] = unvisited;
    for(std::size_t i {0}; i < n_vertices_; ++i) {
        if(ids[i] == unvisited)
            tarjansDfs(i, scc_count,id, stack, ids, on_stack, low);
    }

    return low;
}

void Graph::countInAndOutEulerianPath(std::vector<int>& in, std::vector<int>& out) {
    for(auto& id_vertice: adj_) {
        // Vertices.first gets access to n if n = std::map<n, ...> and
        // Vertices.second gets access to x if x = std::map<..., x>
        for(auto& edge: id_vertice.second) {
            ++in[edge.getValue()];
            ++out[id_vertice.first];
        }
    }
}

// Check wether an eulerian path exists or not
bool Graph::graphHasEulerianPath(const std::vector<int> in, const std::vector<int> out) {
    int start_nodes{0}, end_nodes {0};
    for(int i {0}; i<n_vertices_;++i) {
        if(out[i] - in[i] > 1 || in[i] - out[i] > 1) return false;
        else if(out[i] - in[i] == 1 ) ++start_nodes;
        else if(in[i] - out[i] == 1) ++end_nodes;
    }
    return ((start_nodes==0 && end_nodes==0) || (start_nodes==1 && end_nodes==1));
}

int Graph::findStartNodeEulerianPath(const std::vector<int> in, const std::vector<int> out) {
    int start {0};
    for(int i{0}; i < n_vertices_; ++i) {
        // Unique starting node
        if(out[i]- in[i] == 1) return i;
        // Start at any node with outgoing edge
        if(out[i] > 0) start=i;
    return start;
    }
    return 0;
}


void Graph::dfsEulerianPath(int at, std::vector<int>& in, std::vector<int>& out, std::vector<int>& path) {
    // While the current node still has outgoing edges
    while(out[at] != 0) {
        Edge next_node {adj_[at][--out[at]]};
        dfsEulerianPath(next_node.getValue(), in, out, path);
    }
    path.insert(path.begin(), at);
}

std::vector<int> Graph::eulerianPath() {
    std::vector<int> in (n_vertices_, 0);
    std::vector<int> out (n_vertices_, 0);
    std::vector<int> path {};
    countInAndOutEulerianPath(in, out);
    
    if(!graphHasEulerianPath(in, out)) return path;
    dfsEulerianPath(findStartNodeEulerianPath(in,out), in, out, path);
    
    if(path.size() == n_vertices_ + 1) return path;
    return std::vector<int> {};
}
