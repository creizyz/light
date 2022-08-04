#pragma once

#include "core/optics/shape.hpp"

class Sphere : public Shape
{
public:
    Sphere(const Vec3 & center, const double radius, const std::shared_ptr<Material> material);

    bool hit(const Ray & ray, double tmin, double tmax, Intersection & intersection) const;

private:
    const Vec3 center;
    const double radius;
    const std::shared_ptr<Material> material;
};