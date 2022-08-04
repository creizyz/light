#pragma once

#include "../maths/vec3.hpp"
#include <memory>

class Material;

struct Intersection
{
    double                      t;
    Vec3                        point;
    std::shared_ptr<Material>   material;
    Vec3                        normal;


    Intersection();
    Intersection(const double t, const Vec3 & point, std::shared_ptr<Material> material, const Vec3 & normal);
};