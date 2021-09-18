#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"

#include <map>
#include <vector>
#include <queue>



// Graph class represents a directed graph
// using adjecency list representation
class Graph {
private:
    std::map<int, bool> visited_ {};
    std::map<int, std::vector<Edge>> adj_ {};
    std::vector<std::vector<double>> adjm_ {};
    
    // Functions and subfunctions for topsort
    int dfstopsort(int i, int at, std::vector<int>& ordering);

    // Functions and subfunctions for optiamal Path with dijkstras algo
    std::pair<std::vector<double>, std::vector<double>> dijkstrasOptimalPath(int start, int v);
public:
    // Function to add an edge to graph
    void addEdge(int v,const Edge& edge);

    std::vector<int> topsort(int v);
    
    // Used double vectors because double has infinity property
    std::vector<double> dagShortestPath(int start, int v);
    std::vector<double> dijkstras(int start, int v);
    std::vector<double> mainDijkstrasOptimalPath(int start, int end, int v);
    std::vector<double> bellmanFord(int start, int v);



};

#endif // GRAPH_H
