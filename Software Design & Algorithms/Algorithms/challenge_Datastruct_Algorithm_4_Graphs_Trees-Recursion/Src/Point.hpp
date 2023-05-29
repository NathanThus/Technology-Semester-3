#ifndef POINT_HPP
#define POINT_HPP

#include <bits/stdc++.h>
using std::vector;

class Point
{
    private:
    int ID;
    std::unordered_map<int, Point*> children;

    public:
    Point(int id);
    ~Point();

    int GetID() { return ID; }
    int GetConnections() { return children.size(); }
    
    //DEBUG
    int ExposeConnection() { if(!children.empty()) return children.at(0)->GetID(); else return 0;}
    Point(const Point& other) = delete;
    Point(Point&& other) noexcept;

    //DEBUG


    void AddChild(Point* child);

    /// @brief Recusively dives into the tree to find the shortest path to the goal.
    /// @param paths Vector of all paths to the goal.
    /// @param currentPath Current path length.
    /// @param goal The goal ID.
    void Dive(vector<int> &paths, int currentPath, int goal);

    friend std::ostream& operator<<(std::ostream& os, const Point& point);
};



#endif // POINT_HPP