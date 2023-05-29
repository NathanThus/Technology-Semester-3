#include "Graph.hpp"

void Graph::GenerateChildren(int count)
{
    for(int i = 1; i <= count; i++)
    {
        Points.try_emplace(i, i); // Points.Try_emplace(i Point(i)); -> It parses the constructor values
        /*                ^, ^
                          |  Ctor arguments
                          |
                          Map key (kind of like an index)
        */
    }
}

void Graph::AddConnection(int sourceID, int destinationID)
{
    Points.at(sourceID).AddChild(&Points.at(destinationID));
}

int Graph::GetShortestPath(int goal)
{
    vector<int> paths;
    int currentPath = 0;

    if (Points.empty())
    {
        return -1;
    }

    Points.at(1).Dive(paths, currentPath, goal);

    if(paths.empty())
    {
        return -1;
    }

    int shortestPath = paths.at(0);

    for (size_t i = 0; i < paths.size(); i++)
    {
        if(paths.at(i) < shortestPath)
        {
            shortestPath = paths.at(i);
        }
    }

    return shortestPath;
}