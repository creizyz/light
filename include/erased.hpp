#pragma once

#include <memory>
#include <typeindex>

namespace light
{
    class erased_ptr
    {
    private:
        std::shared_ptr<void> data{ nullptr };
        std::type_index       type{ typeid(void) };

        erased_ptr() = delete;
        erased_ptr(std::shared_ptr<void> && data, std::type_index type);
        erased_ptr(const std::shared_ptr<void> & data, std::type_index type);

    public:
        template <typename T>
        explicit erased_ptr(const std::shared_ptr<T> & ptr);

        template <typename T>
        explicit erased_ptr(std::unique_ptr<T> && ptr);

        template <typename T>
        explicit erased_ptr(T * ptr);

        erased_ptr(const erased_ptr & other) = default;
        erased_ptr(erased_ptr && other) = default;

        erased_ptr & operator=(const erased_ptr & other) = default;
        erased_ptr & operator=(erased_ptr && other) = default;

        template <typename T>
        T & get();

        template <typename T>
        const T & get() const;
    };

    template <typename T, typename... Args>
    erased_ptr make_erased(Args&&... args)
    {
        return erased_ptr(new T(std::forward<Args>(args)...));
    }

    inline erased_ptr::erased_ptr(std::shared_ptr<void> && data, std::type_index type)
        : data{ data }
        , type{ type }
    { }

    inline erased_ptr::erased_ptr(const std::shared_ptr<void> & data, std::type_index type)
        : data{ data }
        , type{ type }
    { }

    template <typename T>
    erased_ptr::erased_ptr(const std::shared_ptr<T> & ptr)
        : data{ static_cast<void*>(ptr.get()), [](void * p) { delete static_cast<T*>(p); } }
        , type{ typeid(T) }
    { }

    template <typename T>
    erased_ptr::erased_ptr(std::unique_ptr<T> && ptr)
        : data{ static_cast<void*>(ptr.release()), [](void * p) { delete static_cast<T*>(p); } }
        , type{ typeid(T) }
    { }

    template <typename T>
    erased_ptr::erased_ptr(T * ptr)
        : data{ static_cast<void*>(ptr), [](void * p) { delete static_cast<T*>(p); } }
        , type{ typeid(T) }
    { }

    template <typename T>
    T & erased_ptr::get()
    {
        if (type != typeid(T))
        {
            throw std::bad_cast();
        }

        return *static_cast<T *>(data.get());
    }

    template <typename T>
    const T & erased_ptr::get() const
    {
        if (type != typeid(T))
        {
            throw std::bad_cast();
        }

        return *static_cast<const T *>(data.get());
    }
}