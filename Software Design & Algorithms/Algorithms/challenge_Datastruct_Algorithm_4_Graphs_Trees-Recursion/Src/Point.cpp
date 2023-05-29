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
    children.clear();
}
    

void Point::AddChild(Point* child)
{
    children.try_emplace(child->GetID(), child);
}

void Point::Dive(vector<int> &paths, int currentPath, int goal)
{
    if(ID == goal)
    {
        cout << "Found it" << endl;
        paths.push_back(currentPath);
        return;
    }
    else
    {
        if(children.empty())
        {
            return;
        }
        for (auto [ID, ptr] : children)
        {
            ptr->Dive(paths, currentPath + 1, goal);
        }
        // for(size_t i = 0; i < children.size(); i++)
        // {
        //     children.at(i)->Dive(paths, currentPath, goal);
        // }
    }
    return;
}

Point::Point(Point &&other) noexcept
    : ID(other.ID)
    , children(std::move(other.children))
{
}
