#pragma once

#include <memory>
#include <cstring>

/**
 * @brief An image channel
 * 
 * @tparam T underlying type of the data stored in the channel
 */
template <typename T>
struct channel
{
public:
    /**
     * @brief Default constructor
     */
    channel()
        : m_data{ nullptr }
        , m_width{ 0 }
        , m_height{ 0 }
    { }

    /**
     * @brief Construct a new channel object
     * 
     * @param width channel width
     * @param height channel height
     */
    channel(size_t width, size_t height)
        : m_data{ new T[width * height] }
        , m_width{ width }
        , m_height{ height }
    { }

    /**
     * @brief Copy constructor
     * 
     * @note this constructor does a deep copy of another channel
     * 
     * @param other channel to copy
     */
    channel(const channel & other)
        : m_data{ new T[other.m_width * other.m_height] }
        , m_width{ other.m_width }
        , m_height{ other.m_height }
    {
        std::memcpy(m_data, other.m_data, sizeof(T) * m_width * m_height);
    }

    /**
     * @brief Move constructor
     * 
     * @param other channel to move data from
     */
    channel(channel && other)
        : m_data{ std::move(other.m_data) }
        , m_width{ std::move(other.m_width) }
        , m_height{ std::move(other.m_height) }
    { }

    /**
     * @brief Copy operator
     * 
     * @note memory allocation only happens if the memory size of
     *       current channel and other channel differ.
     * 
     * @param other channel to copy
     * @return channel<T>& reference to the current channel
     */
    channel<T> & operator=(const channel & other)
    {
        if (m_width * m_height != other.m_width * other.m_height)
        {
            m_data.reset(new T[other.m_width * other.m_height]);
        }
        m_width = other.m_width;
        m_height = other.m_height;
        std::memcpy(m_data, other.m_data, sizeof(T) * m_width * m_height);
        return *this;
    }

    /**
     * @brief Move operator
     * 
     * @param other channel to move data from
     * @return channel<T>& reference to the current channel
     */
    channel<T> & operator=(channel && other)
    {
        m_data.reset(std::move(other.m_data));
        m_width = std::move(other.m_width);
        m_height = std::move(other.m_height);
        return *this;
    }
    
    /**
     * @brief Channel element access
     * 
     * @note accessing an element outsite of the bounds
     *       of the channel results in undefined behaviour
     * 
     * @param x element column index
     * @param y element row index
     * @return T& reference to the element at (x,y)
     */
    T & at(size_t x, size_t y)
    {
        return m_data[y * m_width + x];
    }

    /**
     * @brief Channel element access (const)
     * 
     * @note accessing an element outsite of the bounds
     *       of the channel results in undefined behaviour
     * 
     * @param x element column index
     * @param y element row index
     * @return const T& const reference to the element at (x,y)
     */
    const T & at(size_t x, size_t y) const
    {
        return m_data[y * m_width + x];
    }

    /**
     * @brief Channel raw data access
     * 
     * @return T* pointer to the channel data
     */
    T * data()
    {
        return m_data.get();
    }

    /**
     * @brief Channel raw data access (const)
     * 
     * @return T* pointer to the channel data
     */
    const T * data() const
    {
        return m_data.get();
    }

    /**
     * @brief Checks if the channel is in valid state
     * 
     * @return true 
     * @return false the channel is in invalid state
     */
    bool valid() const
    {
        return m_data && m_width * m_height != 0;
    }

    /**
     * @brief Get channel width
     * 
     * @return size_t channel width
     */
    size_t width() const
    {
        return m_width;
    }

    /**
     * @brief Get channel height
     * 
     * @return size_t channel height
     */
    size_t height() const
    {
        return m_height;
    }

protected:
    std::unique_ptr<T[]> m_data;
    size_t m_width;
    size_t m_height;
};

using Channel = channel<double>;