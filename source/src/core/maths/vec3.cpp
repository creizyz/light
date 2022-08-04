#include "core/maths/vec3.hpp"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3::Vec3()
    : e{ 0, 0, 0 }
{

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3::Vec3(double x, double y, double z)
    : e{ x, y, z }
{

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3 Vec3::randomInUnitSphere()
{
    while (true)
    {
        auto p = Vec3::random(-1, 1);
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3 Vec3::randomUnitVector()
{
    auto a = commons::random(0., 2*commons::constants<double>::pi);
    auto z = commons::random(-1., 1.);
    auto r = sqrt(1 - z*z);
    return Vec3(r * cos(a), r*sin(a), z);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3 Vec3::randomInHemisphere(const Vec3 & normal)
{
    auto inUnitSphere = Vec3::randomInUnitSphere();
    if (dot(inUnitSphere, normal) > commons::constants<double>::precision)
    {
        return inUnitSphere;
    }
    else
    {
        return -inUnitSphere;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3 Vec3::operator-() const
{
    return Vec3(-e[0], -e[1], -e[2]);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3& Vec3::operator+=(const Vec3 & other)
{
    e[0] += other.e[0];
    e[1] += other.e[1];
    e[2] += other.e[2];
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3& Vec3::operator-=(const Vec3 & other)
{
    e[0] -= other.e[0];
    e[1] -= other.e[1];
    e[2] -= other.e[2];
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3& Vec3::operator*=(const Vec3 & other)
{
    e[0] *= other.e[0];
    e[1] *= other.e[1];
    e[2] *= other.e[2];
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3& Vec3::operator/=(const Vec3 & other)
{
    e[0] /= other.e[0];
    e[1] /= other.e[1];
    e[2] /= other.e[2];
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3& Vec3::operator*=(const double s)
{
    e[0] *= s;
    e[1] *= s;
    e[2] *= s;
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Vec3& Vec3::operator/=(const double s)
{
    e[0] /= s;
    e[1] /= s;
    e[2] /= s;
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -