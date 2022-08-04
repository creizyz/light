#pragma once

#include <cmath>

#include "commons.hpp"

class Vec3
{
public:
    Vec3();
    Vec3(double x, double y, double z);

    static Vec3 random();
    static Vec3 random(double min, double max);
    static Vec3 randomInUnitSphere();
    static Vec3 randomUnitVector();
    static Vec3 randomInHemisphere(const Vec3 & normal);

    static Vec3 reflect(const Vec3 & direction, const Vec3 & normal);

    double & x();
    double & y();
    double & z();

    const double & x() const;
    const double & y() const;
    const double & z() const;

    Vec3 operator-() const;

    Vec3 & operator+=(const Vec3 & other);
    Vec3 & operator-=(const Vec3 & other);
    Vec3 & operator*=(const Vec3 & other);
    Vec3 & operator/=(const Vec3 & other);
    Vec3 & operator*=(const double s);
    Vec3 & operator/=(const double s);

    double length() const;
    double lengthSquared() const;
    Vec3 unit() const;

protected:
    double e[3];
};

Vec3 operator+(const Vec3 & u, const Vec3 & v);
Vec3 operator-(const Vec3 & u, const Vec3 & v);
Vec3 operator/(const Vec3 & u, const Vec3 & v);
Vec3 operator*(const Vec3 & u, const Vec3 & v);
Vec3 operator*(const double s, const Vec3 & u);
Vec3 operator*(const Vec3 & u, const double s);
Vec3 operator/(const Vec3 & u, const double s);

double dot(const Vec3 & u, const Vec3 & v);
Vec3 cross(const Vec3 & u, const Vec3 & v);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 Vec3::random()
{
    return Vec3{ commons::random<double>(), commons::random<double>(), commons::random<double>() };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 Vec3::random(double min, double max)
{ 
    return Vec3{ commons::random<double>(min, max), commons::random<double>(min, max), commons::random<double>(min, max) };
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 Vec3::reflect(const Vec3 & direction, const Vec3 & normal)
{
    return direction - 2 * dot(direction, normal) * normal;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline double & Vec3::x()
{
    return this->e[0];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline double & Vec3::y()
{
    return this->e[1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline double & Vec3::z()
{
    return this->e[2];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline const double & Vec3::x() const
{
    return this->e[0];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline const double & Vec3::y() const
{
    return this->e[1];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline const double & Vec3::z() const
{
    return this->e[2];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline double Vec3::length() const
{
    return std::sqrt(lengthSquared());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline double Vec3::lengthSquared() const
{
    return this->e[0] * this->e[0]
         + this->e[1] * this->e[1]
         + this->e[2] * this->e[2];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 Vec3::unit() const
{
    return *this / length();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 operator+(const Vec3 & u, const Vec3 & v)
{
    return Vec3(
        u.x() + v.x(),
        u.y() + v.y(),
        u.z() + v.z()
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 operator-(const Vec3 & u, const Vec3 & v)
{
    return Vec3(
        u.x() - v.x(),
        u.y() - v.y(),
        u.z() - v.z()
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 operator/(const Vec3 & u, const Vec3 & v)
{
    return Vec3(
        u.x() / v.x(),
        u.y() / v.y(),
        u.z() / v.z()
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 operator*(const Vec3 & u, const Vec3 & v)
{
    return Vec3(
        u.x() * v.x(),
        u.y() * v.y(),
        u.z() * v.z()
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 operator*(const Vec3 & u, double s)
{
    return Vec3(
        u.x() * s,
        u.y() * s,
        u.z() * s
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 operator*(const double s, const Vec3 & u)
{
    return Vec3(
        u.x() * s,
        u.y() * s,
        u.z() * s
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 operator/(const Vec3 & u, double s)
{
    return Vec3(
        u.x() / s,
        u.y() / s,
        u.z() / s
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline double dot(const Vec3 & u, const Vec3 & v)
{
    return u.x() * v.x()
         + u.y() * v.y()
         + u.z() * v.z();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline Vec3 cross(const Vec3 & u, const Vec3 & v)
{
    return Vec3(
        u.y() * v.z() - u.z() * v.y(),
        u.z() * v.x() - u.x() * v.z(),
        u.x() * v.y() - u.y() * v.x()
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -