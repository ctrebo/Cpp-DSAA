#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <queue>

// Graph class represents a directed graph
// using adjecency list representation
class Graph {
private:
    std::map<int, bool> visited_ {};
    std::map<int, std::vector<int>> adj_ {};
    
    // Functions and subfunctions for shortestPathBfs
    std::vector<int> solve(int s, int v);
    std::vector<int> reconstructPath(int s, int e,std::vector<int>& prev);

public:
    // Function to add an edge to graph
    void addEdge(int v, int w);
    
    // DFS traversel of vertices
    void dfs(int v);
    void bfs(int v);
    
    std::vector<int> shortestPathBfs(int s, int e, int v);
};

#endif // GRAPH_H
