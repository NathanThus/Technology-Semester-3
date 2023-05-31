#ifndef BRANCH_HPP
#define BRANCH_HPP

#include <bits/stdc++.h>

class Branch
{
    private:
    int ID;
    std::unordered_map<int, Branch*> Connections;
    public:
    Branch(int id);
    void AddConnection(int id, Branch& branch);
    void Dive(std::vector<int>& paths, int currentPath);
};

#endif //BRANCH_HPP