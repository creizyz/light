#pragma once

#include <iostream>

/**
 * @brief Public interface for a real sampler in [0..1]
 */
struct sampler
{
    /**
     * @brief Destroy the sampler object
     */
    virtual ~sampler();

    /**
     * @brief Get the next real in the pseudo-random sequence
     * 
     * @return double a random real in [0..1]
     */
    virtual double next() = 0;
};