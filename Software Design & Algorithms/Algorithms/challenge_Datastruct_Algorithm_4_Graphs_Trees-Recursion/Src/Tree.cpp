#include "Tree.hpp"

void Tree::GenerateBranches(int total)
{
    for (int i = 1; i <= total; i++)
    {
        Branches.emplace(i, i);
    }
    
}

void Tree::AddConnection(int source, int left, int right)
{
    if(left != 0)
    {
        Branches[source].AddConnection(left, Branches[left]);
    }
    if(right != 0)
    {
        Branches[source].AddConnection(right, Branches[right]);
    }
}

int Tree::DeepestPath(int &result)
{
    std::vector<int> paths;
    Branches[1].Dive(paths, 0);
    if(paths.empty())
    {
        result = 0;
        return -1;
    }
    
    result = *std::max_element(paths.begin(), paths.end());
    return 0;
}
