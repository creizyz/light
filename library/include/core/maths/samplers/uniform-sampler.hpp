#pragma once

#include <random>

#include "sampler.hpp"

struct uniform_sampler : sampler
{
    template <typename T>
    uniform_sampler(T seed)
        : generator{ seed }
        , distribution{ 0., 1. }
    { }

    virtual double next() override
    {
        return distribution(generator);
    }

private:
    std::mt19937 generator;
    std::uniform_real_distribution<double> distribution;
};