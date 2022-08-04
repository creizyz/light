#include "renderer/camera.hpp"

Camera::Camera()
    : lowerLeftCorner { -2.0, -1.0,  -1.0 }
    , horizontal      {  4.0,  0.0,   0.0 }
    , vertical        {  0.0,  2.0,   0.0 }
    , origin          {  0.0,  0.0,   0.0 }
{

}

Ray Camera::getRay(const double u, const double v) const
{
    return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
}