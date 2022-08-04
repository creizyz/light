#pragma once

#include <cstddef>
#include <cstdint>

#include "../prime.hpp"

/**
 * @brief Van Der Corput sequence implementation
 * 
 * The Van Der Corput sequence generator of base b is described by the following formula :
 * 
 * \f$ \sum_{i=0}^{N-1} d_{i} \cdot b^{-(i+1)} \f$
 * 
 * This implementation uses the fact that :
 * 
 * \f$ \sum_{i=0}^{N-1} d_{i} \cdot b^{-(i+1)} = \frac{1}{b^{N}} \sum_{i=0}^{N-1} d_{i} \cdot b^{N-(i+1)} \f$
 * 
 * In expanded form :
 * 
 * \f$ d_{0} b^{-1} + d_{1} b^{-2} + ... + d_{N-1} b^{-N} = \frac{d_{0} b^{N-1} + d_{1} b^{N-2} + ... + d_{N-1} b^{0}}{b^{N}} \f$
 * 
 * References :
 * 
 * - <a href='https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/monte-carlo-methods-in-practice/introduction-quasi-monte-carlo'>scratch pixel</a>
 * - <a href='https://www.pbr-book.org/3ed-2018/Sampling_and_Reconstruction/The_Halton_Sampler#RadicalInverseSpecialized'>physically based rendering</a>
 * 
 * @tparam base base of the radical inverse sequence generator
 * @param x index of the number generated
 * @return constexpr double a quasi-random number in [0..1]
 */
template <size_t base>
constexpr double radical_inverse(uint64_t n)
{
    constexpr double inv_base = 1. / base;
    
    double inv_base_acc = inv_base;
    uint64_t mirror_acc = 0;
    while (n)
    {
        uint64_t next  = n / base;
        uint64_t digit = n - next * base;
        
        mirror_acc = mirror_acc * base + digit;
        inv_base_acc *= inv_base;
        
        n = next;
    }
    return mirror_acc * inv_base_acc;
}

/**
 * @brief bit inversion (mirror) in base 2
 * 
 * Reference:
 * 
 * - <a href='https://www.pbr-book.org/3ed-2018/Sampling_and_Reconstruction/The_Halton_Sampler#RadicalInverseSpecialized'>physically based rendering</a>
 * 
 */
constexpr inline explicit uint32_t reverse_bits(uint32_t n)
{
    n = (n << 16) | (n >> 16);
    n = ((n & 0x00ff00ff) << 8) | ((n & 0xff00ff00) >> 8);
    n = ((n & 0x0f0f0f0f) << 4) | ((n & 0xf0f0f0f0) >> 4);
    n = ((n & 0x33333333) << 2) | ((n & 0xcccccccc) >> 2);
    n = ((n & 0x55555555) << 1) | ((n & 0xaaaaaaaa) >> 1);
    return n;
}

/**
 * @brief bit inversion (mirror) in base 2
 * 
 * Reference:
 * 
 * - <a href='https://www.pbr-book.org/3ed-2018/Sampling_and_Reconstruction/The_Halton_Sampler#RadicalInverseSpecialized'>physically based rendering</a>
 * 
 */
constexpr inline explicit uint64_t reverse_bits(uint64_t n)
{
    uint64_t n0 = static_cast<uint64_t>(reverse_bits(static_cast<uint32_t>(n)));
    uint64_t n1 = static_cast<uint64_t>(reverse_bits(static_cast<uint32_t>(n >> 32)));
    return (n0 << 32) | n1;
}

/**
 * @brief Accelleration of the Van Der Corput sequence for base 2
 * 
 * Reference:
 * 
 * - <a href='https://www.pbr-book.org/3ed-2018/Sampling_and_Reconstruction/The_Halton_Sampler#RadicalInverseSpecialized'>physically based rendering</a>
 * 
 * @param n index of the number generated
 * @return constexpr double a quasi-random number in [0..1]
 */
template <>
constexpr inline double radical_inverse<2>(uint64_t n)
{
    return reverse_bits(n) * 0x1p-64;
}

/**
 * @brief Van Der Corput generator for multi-dimensions
 * 
 * Bases used for the van der corput are selected using the prime number sequence
 * 
 * Reference:
 * 
 * - <a href='https://www.pbr-book.org/3ed-2018/Sampling_and_Reconstruction/The_Halton_Sampler#RadicalInverseSpecialized'>physically based rendering</a>
 * 
 * @tparam dimension dimension for which we want to generate a number
 * @param n index of the number generated
 * @return constexpr double a quasi-random number in [0..1]
 */
template <size_t dimension>
    requires (dimension < 1000)
constexpr inline double multidim_radical_inverse(uint64_t n)
{
    return radical_inverse<prime(dimension)>(n);
}