#ifndef POINT_HPP
#define POINT_HPP

#include <bits/stdc++.h>
using std::vector;

class Point
{
    private:
    int ID;
    std::unordered_map<int, Point*> connections;
    int distance = 0;

    public:
    Point(int id);
    Point(const Point& other) = delete;
    Point(Point&& other) noexcept;
    ~Point();

    int GetID() { return ID; }
    int GetDistance() { return distance; }
    std::unordered_map<int, Point*>& GetConnections() { return connections; }
    
    void AddChild(Point* child);
    void SetDistance(int distance);

    friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

#endif // POINT_HPP
