#pragma once

#include "channel.hpp"
#include "color.hpp"

/**
 * @brief An RGB image
 * 
 * @tparam T underlying type for RGB components of each pixel of the image
 */
template <typename T>
struct image
{
    /**
     * @brief Default constructor
     */
    image()
        : m_R{ }
        , m_G{ }
        , m_B{ }
    { }

    /**
     * @brief Construct a new image object
     * 
     * @param width width of the image
     * @param height height of the image
     */
    image(size_t width, size_t height)
        : m_R{ width, height }
        , m_G{ width, height }
        , m_B{ width, height }
    { }

    /**
     * @brief Copy constructor
     * 
     * @param other image to copy
     */
    image(const image<T> & other)
        : m_R{ other.m_R }
        , m_G{ other.m_G }
        , m_B{ other.m_B }
    { }

    /**
     * @brief Move constructor
     * 
     * @param other image to move
     */
    image(image<T> && other)
        : m_R{ std::move(other.m_R) }
        , m_G{ std::move(other.m_G) }
        , m_B{ std::move(other.m_B) }
    { }

    /**
     * @brief Copy operator
     * 
     * @param other image to copy
     * @return image<T>& reference to current image
     */
    image<T> & operator=(const image<T> & other)
    {
        m_R = other.m_R;
        m_G = other.m_G;
        m_B = other.m_B;
        return *this;
    }

    /**
     * @brief Move operator
     * 
     * @param other image to move
     * @return image<T>& reference to current image
     */
    image<T> & operator=(image<T> && other)
    {
        m_R = std::move(other.m_R);
        m_G = std::move(other.m_G);
        m_B = std::move(other.m_B);
        return *this;
    }

    /**
     * @brief Sets RGB value of a pixel in the image
     * 
     * @param x pixel column index
     * @param y pixel row index
     * @param c color to set
     */
    void set(size_t x, size_t y, const color<T> & c)
    {
        m_R.at(x, y) = c.r();
        m_G.at(x, y) = c.g();
        m_B.at(x, y) = c.b();
    }

    /**
     * @brief get color value of a pixel
     * 
     * @param x pixel column index
     * @param y pixel row index
     * @return color<T> color of the pixel
     */
    color<T> at(size_t x, size_t y) const
    {
        return {
            m_R.at(x, y),
            m_G.at(x, y),
            m_B.at(x, y)
        };
    }

    /**
     * @brief access red channel
     * 
     * @return channel<T>& reference to red channel
     */
    channel<T> & r()
    {
        return m_R;
    }

    /**
     * @brief access green channel
     * 
     * @return channel<T>& reference to green channel
     */
    channel<T> & g()
    {
        return m_G;
    }
    
    /**
     * @brief access blue channel
     * 
     * @return channel<T>& reference to blue channel
     */
    channel<T> & b()
    {
        return m_B;
    }

    /**
     * @brief access red channel (const)
     * 
     * @return channel<T>& const reference to red channel
     */
    const channel<T> & r() const
    {
        return m_R;
    }
    
    /**
     * @brief access green channel (const)
     * 
     * @return channel<T>& const reference to green channel
     */
    const channel<T> & g() const
    {
        return m_G;
    }
    
    /**
     * @brief access blue channel (const)
     * 
     * @return channel<T>& const reference to blue channel
     */
    const channel<T> & b() const
    {
        return m_B;
    }

    /**
     * @brief Get image width
     * 
     * @return size_t image width
     */
    size_t width() const
    {
        return m_G.width();
    }

    /**
     * @brief image width
     * 
     * @return size_t 
     */
    size_t height() const
    {
        return m_G.height();
    }

protected:
    channel<T> m_R;
    channel<T> m_G;
    channel<T> m_B;
};

using Image = image<double>;
