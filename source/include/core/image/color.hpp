#pragma once

/**
 * @brief An RGB color
 * 
 * @tparam T underlying type of RGB components
 */
template <typename T>
struct color
{
    /**
     * @brief Default constructor
     */
    color()
        : m_data{ {}, {}, {} }
    { }

    /**
     * @brief Construct a new color object
     * 
     * @param r red component
     * @param g green component
     * @param b blue component
     */
    color(T r, T g, T b)
        : m_data{ r, g, b }
    { }

    /**
     * @brief access red element
     * 
     * @return T& reference to red element
     */
    T & r() { return m_data[0]; }

    /**
     * @brief access green element
     * 
     * @return T& reference to green element
     */
    T & g() { return m_data[1]; }

    /**
     * @brief access blue element
     * 
     * @return T& reference to blue element
     */
    T & b() { return m_data[2]; }

    /**
     * @brief acces red element (const)
     * 
     * @return const T& const reference to red element
     */
    const T & r() const { return m_data[0]; }

    /**
     * @brief access green element (const)
     * 
     * @return T& const reference to greeb element
     */
    const T & g() const { return m_data[1]; }

    /**
     * @brief access blue element (const)
     * 
     * @return T& const reference to blue element
     */
    const T & b() const { return m_data[2]; }

    
    // TODO: documentation
    
    color<T> operator-() const
    {
        return {
            -m_data[0],
            -m_data[1],
            -m_data[2]
        };
    }

    color<T> & operator+=(const color<T> & other)
    {
        m_data[0] += other.m_data[0];
        m_data[1] += other.m_data[1];
        m_data[2] += other.m_data[2];
        return *this;
    }

    color<T> & operator-=(const color<T> & other)
    {
        m_data[0] -= other.m_data[0];
        m_data[1] -= other.m_data[1];
        m_data[2] -= other.m_data[2];
        return *this;
    }

    color<T> & operator*=(const color<T> & other)
    {
        m_data[0] *= other.m_data[0];
        m_data[1] *= other.m_data[1];
        m_data[2] *= other.m_data[2];
        return *this;
    }

    color<T> & operator/=(const color<T> & other)
    {
        m_data[0] /= other.m_data[0];
        m_data[1] /= other.m_data[1];
        m_data[2] /= other.m_data[2];
        return *this;
    }

    color<T> & operator*=(const double s)
    {
        m_data[0] *= s;
        m_data[1] *= s;
        m_data[2] *= s;
        return *this;
    }

    color<T> & operator/=(const double s)
    {
        m_data[0] /= s;
        m_data[1] /= s;
        m_data[2] /= s;
        return *this;
    }

protected:
    T m_data[3];
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
inline color<T> operator+(const color<T> & u, const color<T> & v)
{
    return color<T>(
        u.r() + v.r(),
        u.g() + v.g(),
        u.b() + v.b()
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
inline color<T> operator-(const color<T> & u, const color<T> & v)
{
    return color<T>(
        u.r() - v.r(),
        u.g() - v.g(),
        u.b() - v.b()
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
inline color<T> operator/(const color<T> & u, const color<T> & v)
{
    return color<T>(
        u.r() / v.r(),
        u.g() / v.g(),
        u.b() / v.b()
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
inline color<T> operator*(const color<T> & u, const color<T> & v)
{
    return color<T>(
        u.r() * v.r(),
        u.g() * v.g(),
        u.b() * v.b()
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
inline color<T> operator*(const color<T> & u, double s)
{
    return color<T>(
        u.r() * s,
        u.g() * s,
        u.b() * s
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
inline color<T> operator*(const double s, const color<T> & u)
{
    return color<T>(
        u.r() * s,
        u.g() * s,
        u.b() * s
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T>
inline color<T> operator/(const color<T> & u, double s)
{
    return color<T>(
        u.r() / s,
        u.g() / s,
        u.b() / s
    );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

using Color = color<double>;