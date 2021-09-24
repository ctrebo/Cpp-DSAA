#include "graph.h"

#include <iostream>
#include <algorithm>
#include <exception>
#include <limits>
#include <algorithm>
#include <utility>
#include <queue>
#include <cstddef>



typedef std::pair<int, int> pi;

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

int Graph::dfstopsort(int i, int at, std::vector<int>& ordering) {
    visited_[at] = true;
    for(auto element: adj_[at]) {
        if(!visited_[element.getValue()])
            i = dfstopsort(i, element.getValue(), ordering);
    }
    ordering.insert(ordering.begin(), at);
    return (i-1);
}

std::vector<int> Graph::topsort() {
    // i is int because if n_vertices_ == 0, there would be overflow
    int i {static_cast<int>(n_vertices_)-1};
    std::vector<int> ordering {};
    for(std::size_t at{0}; at<n_vertices_; ++at) {
        if(!visited_[at]) {
            i = dfstopsort(i, at, ordering);
        }
    }
     
    cleanVisited();
    return ordering; 
}


std::vector<double> Graph::dagShortestPath(int start) {
    std::vector<int> topOrder {topsort()};
    std::vector<double> dist {};
    for(std::size_t i{0}; i < n_vertices_; ++i) {
        dist.push_back(std::numeric_limits<double>::infinity());
    }
    dist[start]=0;

    for(std::size_t i{0}; i < n_vertices_; ++i) {
        int node_index {topOrder[i]};
        std::cout << node_index << ' ';
        if(dist[node_index] != std::numeric_limits<double>::infinity()) {
            for(auto element: adj_[node_index]) {
                double new_dist {dist[node_index] + element.getWeight()};
                if(dist[element.getValue()]==std::numeric_limits<double>::infinity()) dist[element.getValue()] = new_dist;
                else dist[element.getValue()] = std::min(dist[element.getValue()], new_dist);
            }

        }
    }
    std::cout << '\n';
    cleanVisited();
    return dist;
}


// Pi = std::pair<int, int>
std::vector<double> Graph::dijkstras(int start) {
    std::priority_queue<pi, std::vector<pi>, std::greater<pi> > pq;
    std::vector<double> dist {};
    for(std::size_t i{0}; i < n_vertices_; ++i) {
        dist.push_back(std::numeric_limits<double>::infinity());
    }
    dist[start]=0;
    pq.push(std::make_pair(start, 0));
    while(!pq.empty()) {
       int index {std::get<0>(pq.top())};
       int min_value {std::get<1>(pq.top())};
       pq.pop();
       visited_[index] = true;
       if(dist[index] < min_value)  continue;
       for(auto element: adj_[index]) {
           if(visited_[element.getValue()]) continue;
           int new_dist = dist[index] + element.getWeight();
           if(new_dist < dist[element.getValue()]) {
               dist[element.getValue()] = new_dist;
               pq.push(std::make_pair(element.getValue(), new_dist));
           }
       }
    }
    cleanVisited();
    return dist;
}


std::pair<std::vector<double>, std::vector<double>> Graph::dijkstrasOptimalPath(int start) {
    std::priority_queue<pi, std::vector<pi>, std::greater<pi> > pq;
    std::vector<double> dist {};
    for(std::size_t i{0}; i < n_vertices_; ++i) {
        dist.push_back(std::numeric_limits<double>::infinity());
    }
    std::vector<double> prev;
    for(std::size_t i{0}; i<n_vertices_; ++i) {
        prev.push_back(-1);
    }
    dist[start]=0;
    pq.push(std::make_pair(start, 0));
    while(!pq.empty()) {
       int index {std::get<0>(pq.top())};
       int min_value {std::get<1>(pq.top())};
       pq.pop();
       visited_[index] = true;
       if(dist[index] < min_value)  continue;
       for(auto element: adj_[index]) {
           if(visited_[element.getValue()]) continue;
           int new_dist = dist[index] + element.getWeight();
           if(new_dist < dist[element.getValue()]) {
               prev[element.getValue()] = index;
               dist[element.getValue()] = new_dist;
               pq.push(std::make_pair(element.getValue(), new_dist));
           }
       }
    }
    return std::make_pair(dist, prev);
}

std::vector<double> Graph::mainDijkstrasOptimalPath(int start, int end) {
    std::pair<std::vector<double>, std::vector<double>> var {dijkstrasOptimalPath(start)};
    std::vector<double> dist {std::get<0>(var)};
    std::vector<double> prev {std::get<1>(var)};
    std::vector<double> path {};
    if(dist[end] == std::numeric_limits<double>::infinity()) return path;
    for(double at=end; at != -1; at=prev[at])
        path.push_back(at);
    std::reverse(path.begin(), path.end());
    cleanVisited();
    return path;

}


std::vector<double> Graph::bellmanFord(int start) {
    std::vector<double> dist {};
    for(std::size_t i{0}; i < n_vertices_; ++i) {
        dist.push_back(std::numeric_limits<double>::infinity());
    }
    dist[start] =0;
    for(std::size_t i{0}; i< n_vertices_-1; ++i) {
        for(std::size_t j {0}; j<n_vertices_-1; ++j) {
            for(auto element: adj_[j]) {
                if(dist[j] + element.getWeight()< dist[element.getValue()]) {
                    dist[element.getValue()] = dist[j] + element.getWeight();
                }
            }
        }
    }
    // Repeat to find nodes caught in negative cycles
    for(std::size_t i{0}; i< n_vertices_-1; ++i) {
        for(std::size_t j {0}; j<n_vertices_-1; ++j) {
            for(auto element: adj_[j]) {
                if(dist[j] + element.getWeight()< dist[element.getValue()]) {
                    dist[element.getValue()] = dist[j] + element.getWeight();
                }
            }
        }
    }
    cleanVisited();
    return dist;
}


std::size_t Graph::get_num_vertices() {
    return n_vertices_;
}
