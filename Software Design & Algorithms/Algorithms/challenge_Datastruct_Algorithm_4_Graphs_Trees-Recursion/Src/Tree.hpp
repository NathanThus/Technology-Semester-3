#ifndef TREE_HPP
#define TREE_HPP

#include <bits/stdc++.h>
#include "Branch.hpp"

class Tree
{
    private:
    std::unordered_map<int, Branch> Branches;
    public:
    Tree() = default;
    
    void GenerateBranches(int total);
    void AddConnection(int source, int left, int right);

    int DeepestPath(int& result);
};

#endif //TREE_HPP
