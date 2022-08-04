#include "renderer/shape-list.hpp"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ShapeList::ShapeList()
    : list{}
{

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ShapeList::add(const std::shared_ptr<Shape> shape)
{
    list.push_back(shape);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void ShapeList::clear()
{
    list.clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool ShapeList::hit(const Ray & ray, double tmin, double tmax, Intersection & intersection) const
{
    Intersection tmp;
    bool hitAnything{ false };
    double closest{ tmax };

    for (const auto & shape : list)
    {
        if (shape->hit(ray, tmin, closest, tmp))
        {
            intersection = tmp;
            hitAnything = true;
            closest = tmp.t;
        }
    }

    return hitAnything;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -