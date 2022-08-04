#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "core/image/image.hpp"
#include "core/image/tone-mapping.hpp"
#include "core/maths/commons.hpp"

namespace ppm_io
{
    namespace impl
    {
        constexpr int PPMImage_MAX_VALUE = 255;
        constexpr int PPMImage_MAX_VALUE_PLUS_1 = PPMImage_MAX_VALUE + 1;

        template <typename T>
        constexpr int ppm_write_conversion(T value)
        {
            return static_cast<int>(PPMImage_MAX_VALUE_PLUS_1 * commons::clamp(value, static_cast<T>(0.), static_cast<T>(0.999)));
        }
    }

    template <typename T>
    bool write(const std::string & path, const image<T> & image, const tone_mapping<T> & tone)
    {
        std::cout << "[ppm-image-io] writing image" << std::flush;
        std::ofstream file{ path, std::ofstream::out | std::ofstream::trunc };
        
        if (file.is_open())
        {
            file << "P3" << std::endl << image.width() << ' ' << image.height() << '\n' << impl::PPMImage_MAX_VALUE_PLUS_1 << '\n';
            for (unsigned int jj =  0; jj < image.height(); jj++)
            {
                unsigned int j = image.height() - 1 - jj;
                std::cout << "\r[ppm-image-io] scanlines remaining: " << j << ' ' << std::flush;
                for (unsigned int i = 0; i < image.width(); i++)
                {
                    const int ir = impl::ppm_write_conversion(tone(image.r().at(i, j)));
                    const int ig = impl::ppm_write_conversion(tone(image.g().at(i, j)));
                    const int ib = impl::ppm_write_conversion(tone(image.b().at(i, j)));
                    file << ir << ' ' << ig << ' ' << ib << '\n';
                }
            }
            std::cout << "\r[ppm-image-io] image writing : success" << std::endl;
            return true;
        }
        else
        {
            std::cout << "\r[ppm-image-io] image writing : failure" << std::endl;
            return false;
        }
    }
}