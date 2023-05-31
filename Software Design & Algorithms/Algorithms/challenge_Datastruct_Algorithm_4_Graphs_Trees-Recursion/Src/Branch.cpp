#include "Branch.hpp"

Branch::Branch(int id)
    : ID(id)
{
}

void Branch::Dive(std::vector<int> &paths, int currentPath)
{
    if(Connections.empty())
    {
        paths.push_back(currentPath);
        return;
    }
    for (auto& connection : Connections)
    {
        connection.second->Dive(paths, currentPath + 1);
    }
}

void Branch::AddConnection(int id, Branch& branch)
{
    Connections.emplace(id, branch);
}
