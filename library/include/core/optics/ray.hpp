#pragma once

#include "../maths/vec3.hpp"

class Ray
{
public:
    Ray();
    Ray(const Vec3 & origin, const Vec3 & direction);

    Vec3 at(double t) const;

    Vec3 origin;
    Vec3 direction;
};