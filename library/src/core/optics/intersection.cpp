#include "core/optics/intersection.hpp"


Intersection::Intersection()
    : t{ 0 }
    , point{}
    , material{}
    , normal{}
{

}

Intersection::Intersection(const double t, const Vec3 & point, std::shared_ptr<Material> material, const Vec3 & normal)
    : t{ t }
    , point{ point }
    , material{ material }
    , normal{ normal }
{

}
