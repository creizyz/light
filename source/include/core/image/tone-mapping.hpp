#pragma once

#include <cmath>

// TODO: comments

template <typename T>
class tone_mapping
{
public:
    virtual ~tone_mapping() { }
    virtual T operator()(const T val) const = 0;
};

template <typename T>
class gamma : public tone_mapping<T>
{
public:
    gamma(const T gamma)
        : scale{ static_cast<T>(1.0 / gamma) }
    { }

    T operator()(const T val) const
    {
        return pow(val, scale);
    }

private:
    T scale;
};

using ToneMapping = tone_mapping<double>;
using Gamma = gamma<double>;