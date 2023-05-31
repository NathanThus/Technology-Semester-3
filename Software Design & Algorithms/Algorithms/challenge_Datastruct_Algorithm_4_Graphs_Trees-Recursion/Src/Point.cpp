#include "Point.hpp"

#include <iostream>
using std::cout;
using std::endl;

Point::Point(int id)
    : ID(id)
{
}

Point::~Point()
{
    connections.clear();
}
    

void Point::AddChild(Point* child)
{
    connections.try_emplace(child->GetID(), child);
}

void Point::SetDistance(int distance)
{
    if(distance < this->distance || this->distance == 0)
    {
        this->distance = distance;
    }
}

Point::Point(Point &&other) noexcept
    : ID(other.ID)
    , connections(std::move(other.connections))
{
}
