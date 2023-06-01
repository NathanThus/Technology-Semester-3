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

int Graph::AddConnection(int sourceID, int destinationID)
{
    if(sourceID == destinationID)
    {
        return -1;
    }
    Points.at(sourceID).AddChild(&Points.at(destinationID));
    Points.at(destinationID).AddChild(&Points.at(sourceID));
    return 0;
}

int Graph::Search(int goal)
{
    if(Points.empty())
    {
        return -1;
    }

    std::queue<Point*> queue;
    std::set<int> visited;

    queue.push(&Points.at(1));

    while(!queue.empty())
    {
        Point* current = queue.front();
        queue.pop();

        visited.insert(current->GetID()); // Now we say we were here

        for(auto& child : current->GetConnections())
        {
            if(!visited.count(child.first)) // If we haven't been here
            {
                // Distance calcs
                child.second->SetDistance(current->GetDistance() + 1);
                queue.push(child.second);
            }
        }
    }
    return Points.at(goal).GetDistance();
}
