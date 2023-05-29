#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <bits/stdc++.h>
#include "Point.hpp"

class Graph
{
private:
    std::unordered_map<int, Point> Points;
public:
    Graph() = default;
    Point& GetChildByID(int id) { return Points.at(id); }
    
    int GetNumberOfPoints() { return Points.size(); }

    // DEBUG
    Graph(const Graph& other) = delete;
    // DEBUG

    void GenerateChildren(int count);
    void AddConnection(int sourceID, int destinationID);
    int GetShortestPath(int goal);
};

#endif // GRAPH_HPP