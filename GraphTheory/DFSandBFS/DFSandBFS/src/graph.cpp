#include "graph.h"

#include <iostream>
#include <algorithm>
#include <exception>


void Graph::addEdge(int v, int w) {
    if(v < 0 || w < 0)
        throw std::length_error("v or w can not be smaller than 0");
    adj_[v].push_back(w);
}

void Graph::dfs(int v) {
    if(visited_[v]) return;
    std::cout << v << ' ';
    visited_[v] = true;

    for(auto element: adj_[v])
        dfs(element);
}

void Graph::bfs(int v) {
    std::queue<int> qu {};
    qu.push(v);
    visited_[v] = true;
    while(!qu.empty()) {
        int current {qu.front()};
        std::cout << current << ' ';
        for(auto element: adj_[current]) {
            if(!visited_[element]) {
                qu.push(element);
                visited_[element] = true;

            }
        }
        qu.pop();
    }
}


std::vector<int> Graph::solve(int s, int v) {
    std::queue<int> q {};
    q.push(s);
    std::vector<int> prev;
    for(int i {0}; i<v; ++i) {
        prev.push_back(-1);
    }
    while(!q.empty()) {
        int current {q.front()}; 
        for(auto element: adj_[current]) {
            if(!visited_[element]) {
                q.push(element);
                visited_[element] = true;
                prev[element] = current;
            }
        }
        q.pop();
    }
    return prev;
}

std::vector<int> Graph::reconstructPath(int s, int e, std::vector<int>& prev) {
    std::vector<int> path {};
    for(int at {e}; at != -1; at=prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    if(path[0] == s)
        return path;
    return std::vector<int> {};
}

std::vector<int> Graph::shortestPathBfs(int s, int e, int v) {
    std::vector<int> prev {solve(s, v)};
    return reconstructPath(s, e, prev);
}


