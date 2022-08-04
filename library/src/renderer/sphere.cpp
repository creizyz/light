#include "renderer/sphere.hpp"

#include <cmath>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Sphere::Sphere(const Vec3 & center, const double radius, const std::shared_ptr<Material> material)
    : center{ center }
    , radius{ radius }
    , material{ material }
{

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool Sphere::hit(const Ray & ray, double tmin, double tmax, Intersection & intersection) const
{
    Vec3 oc = ray.origin - center;
    auto a = dot(ray.direction, ray.direction);
    auto half_b = dot(oc, ray.direction);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant > 0)
    {
        auto root = sqrt(discriminant);
        auto solution = (-half_b - root) / a;
        if (commons::is_inside(solution, tmin, tmax))
        {
            intersection.t = solution;
            intersection.point = ray.at(solution);
            intersection.normal = (intersection.point - center) / radius;
            intersection.material = material;
            return true;
        }
        solution =   (-half_b + root) / a;
        if (commons::is_inside(solution, tmin, tmax))
        {
            intersection.t = solution;
            intersection.point = ray.at(solution);
            intersection.normal = (intersection.point - center) / radius;
            intersection.material = material;
            return true;
        }
    }
    return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
