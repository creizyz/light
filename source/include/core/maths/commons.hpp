#pragma once

#include <limits>
#include <cstdlib>
#include <type_traits>

namespace commons
{
    template <typename T>
    struct constants
    {
        static constexpr T pi = static_cast<T>(3.14159265358979323846);
        static constexpr T precision = 1.0e-8; // TODO: find better solution
        static constexpr T infinity = std::numeric_limits<T>::max();
    };

    template <typename T>
    constexpr bool is_inside(const T val, const T min, const T max)
    {
        return (min + constants<T>::precision <= val && val <= max - constants<T>::precision);
    }

    template <typename T>
    constexpr T degreeToRadian(T degree)
    {
        constexpr double deg_to_rad = constants<double>::pi / 180;
        if (std::is_integral_v<T>)
        {
            return static_cast<T>(degree * deg_to_rad + 0.5);
        }
        else
        {
            return degree * deg_to_rad;
        }
    }

    template <typename T>
    constexpr T radianToDegree(T radian)
    {
        constexpr double rad_to_deg = 180 / constants<double>::pi;
        if (std::is_integral_v<T>)
        {
            return static_cast<T>(radian * rad_to_deg + 0.5);
        }
        else
        {
            return radian * rad_to_deg;
        }
    }

    template <typename T>
    constexpr T min(T a, T b)
    {
        return (a <= b) ? a : b;
    }

    template <typename T>
    constexpr T max(T a, T b)
    {
        return (a >= b) ? a : b;
    }

    /**
     * @brief generates a random value in [0..1]
     * 
     * @return constexpr T random value
     */
    template <typename T>
    constexpr T random() 
    {
        if (std::is_integral_v<T>)
        {
            return static_cast<T>(rand() / (RAND_MAX + 1.0) + 0.5);
        }
        else
        {
            return static_cast<T>(rand() / (RAND_MAX + 1.0));
        }
    }

    /**
     * @brief generates a random value in [min..max]
     * 
     * @param min min random value
     * @param max max random value
     * @return constexpr T random value
     */
    template <typename T>
    constexpr T random(const T min, const T max)
    {
        const double x = rand() / (RAND_MAX + 1.0);
        if (std::is_integral_v<T>)
        {
            return min + static_cast<T>((max - min) * x + 0.5);
        }
        else
        {
            return min + static_cast<T>((max - min) * x);
        }
    }

    /**
     * @brief clamps a value between two limits
     * 
     * @param x value to clamp
     * @param min min value
     * @param max max value
     * @return constexpr T clamped value
     */
    template <typename T>
    constexpr T clamp(T x, T min, T max)
    {
        const T v = (x < min) ? min : x;
        return (v > max) ? max : v;
    }
}