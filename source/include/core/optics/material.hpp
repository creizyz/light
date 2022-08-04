#pragma once

#include "core/image/color.hpp"
#include "ray.hpp"
#include "intersection.hpp"

class Material
{
public:
    virtual ~Material();
    virtual bool scatter(const Ray & in, const Intersection & intersection, Color & attenuation, Ray & out) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const Color & albedo);
    virtual bool scatter(const Ray & in, const Intersection & intersection, Color & attenuation, Ray & out) const;

    const Color albedo;
};

class Metal : public Material
{
public:
    Metal(const Color & albedo);
    virtual bool scatter(const Ray & in, const Intersection & intersection, Color & attenuation, Ray & out) const;

    const Color albedo;
};