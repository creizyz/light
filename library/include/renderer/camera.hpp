#pragma once

#include "core/maths/vec3.hpp"
#include "core/optics/ray.hpp"

class Camera
{
public:
    Camera();

    Ray getRay(const double u, const double v) const;

private:
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 origin;
};