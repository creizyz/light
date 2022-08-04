#pragma once

#include "../maths/vec3.hpp"
#include "intersection.hpp"
#include "ray.hpp"

class Shape
{
public:
    virtual ~Shape();
    virtual bool hit(const Ray & ray, double tmin, double tmax, Intersection & intersection) const = 0;
};