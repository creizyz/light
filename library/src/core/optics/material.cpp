#include "core/optics/material.hpp"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Material::~Material()
{

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Lambertian::Lambertian(const Color & albedo)
    : albedo{ albedo }
{

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool Lambertian::scatter(const Ray & /*in*/, const Intersection & intersection, Color & attenuation, Ray & out) const
{
    out.origin = intersection.point;
    out.direction = intersection.normal + Vec3::randomUnitVector();
    attenuation = albedo;
    return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Metal::Metal(const Color & albedo)
    : albedo(albedo)
{

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool Metal::scatter(const Ray & in, const Intersection & intersection, Color & attenuation, Ray & out) const
{
    out.origin = intersection.point;
    out.direction = Vec3::reflect(in.direction.unit(), intersection.normal);
    attenuation = albedo;
    return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
