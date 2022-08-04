#pragma once

#include "core/optics/shape.hpp"

#include <memory>
#include <vector>

class ShapeList : public Shape
{
public:
    ShapeList();

    void clear();
    void add(const std::shared_ptr<Shape> shape);

    bool hit(const Ray & ray, double tmin, double tmax, Intersection & intersection) const;

private:
    std::vector<std::shared_ptr<Shape>> list;
};