#include "Tree.hpp"

constexpr int MaximumNumberOfBranches = 20;

void Tree::GenerateBranches()
{
    for (int i = 1; i <= MaximumNumberOfBranches; i++)
    {
        Branches.try_emplace(i, i);
    }
    
}

void Tree::AddConnection(int source, int left, int right)
{
    if(left != 0)
    {
        Branches.at(source).AddConnection(left, &Branches.at(left));
    }
    if(right != 0)
    {
        Branches.at(source).AddConnection(right, &Branches.at(right));
    }
}

int Tree::DeepestPath(int &result)
{
    std::vector<int> paths;

    if(Branches.size() < 2)
    {
        return -1;
    }

    Branches.at(1).Dive(paths, 0); // Remind me to ONLY use at()
    
    if(paths.empty())
    {
        result = 0;
        return -1;
    }

    result = *std::max_element(paths.begin(), paths.end());
    return 0;
}
