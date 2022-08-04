#include <iostream>

#include "core/core.hpp"
#include "renderer/shape-list.hpp"
#include "renderer/sphere.hpp"
#include "renderer/camera.hpp"

Color ray_color(const Ray & ray, const Shape & scene, const int depthToLive)
{
    Intersection i;
    if (depthToLive <= 0)
    {
        return Color();
    }
    if (scene.hit(ray, 0, commons::constants<double>::infinity, i))
    {
        Ray scattered;
        Color attenuation;

        if (i.material->scatter(ray, i, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, scene, depthToLive - 1);
        }
        else
        {
            return Color();
        }
    }
    Vec3 direction = ray.direction.unit();
    auto t = 0.5 * (direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main(int argc, char ** argv) {
    if (argc == 2)
    {
        Image image{ 400, 200 };
        const int maxDepth = 500;
        const unsigned int samplesPerPixel = 100;
        const double scale = 1.0 / static_cast<double>(samplesPerPixel);

        ShapeList scene;
        scene.add(std::make_shared<Sphere>(Vec3( 0,    0.0, -1), 0.5, std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3))));
        scene.add(std::make_shared<Sphere>(Vec3( 1,    0.0, -1), 0.5, std::make_shared<Metal>     (Color(0.8, 0.6, 0.2))));
        scene.add(std::make_shared<Sphere>(Vec3(-1,    0.0, -1), 0.5, std::make_shared<Metal>     (Color(0.8, 0.8, 0.8))));
        scene.add(std::make_shared<Sphere>(Vec3( 0, -100.5, -1), 100, std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0))));

        Camera camera;
        // init default image;
        for (unsigned int jj =  0; jj < image.height(); jj++)
        {
            unsigned int j = image.height() - 1 - jj;
            for (unsigned int i = 0; i < image.width(); i++)
            {
                Color c;
                for (unsigned int n = 0; n < samplesPerPixel; n++)
                {
                    const auto u = double(i + commons::random<double>()) / image.width();
                    const auto v = double(j + commons::random<double>()) / image.height();
                    Ray r = camera.getRay(u, v);
                    c += scale * ray_color(r, scene, maxDepth);
                }
                image.set(i, j, c);
            }
        }
        
        ppm_io::write(argv[1], image, Gamma(2.2));

        exit(EXIT_SUCCESS);
    }
    else
    {
        std::cerr << "unknown ouput file" << std::endl;
        exit(EXIT_FAILURE);
    }
}