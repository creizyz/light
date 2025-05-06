#pragma once

#include <memory>
#include <typeindex>

namespace light
{
    class erased_ptr
    {
        std::shared_ptr<void> data{ nullptr };
        std::type_index       type{ typeid(void) };

    public:
        erased_ptr() = delete;

        /**
         * @brief Constructs an erased_ptr as a copy of another erased_ptr.
         *
         * Creates a new instance of `erased_ptr` by copying the state of the given `other` erased_ptr.
         * The managed object and its associated type information are duplicated.
         *
         * @param other The erased_ptr instance to copy from.
         */
        erased_ptr(const erased_ptr & other) noexcept = default;

        /**
         * @brief Constructs an erased_ptr by moving from another erased_ptr.
         *
         * Transfers ownership of the managed object and its associated type information
         * from the given erased_ptr to the newly created instance. The original erased_ptr
         * is left in a valid but unspecified state.
         *
         * @param other The erased_ptr instance to move from.
         */
        erased_ptr(erased_ptr && other) noexcept
            : data{ std::move(other.data) }
            , type{ other.type }
        {
            other.type = typeid(void);
        }

        /**
         * @brief Assigns the state of another erased_ptr to this instance.
         *
         * Performs a copy assignment from the given `erased_ptr` to this instance.
         * Both managed object and stored type information are copied. The previous
         * state of this instance is replaced by the state of the `other` erased_ptr.
         *
         * @param other The erased_ptr to copy from.
         * @return A reference to the updated erased_ptr instance.
         */
        erased_ptr & operator=(const erased_ptr & other) noexcept = default;

        /**
         * @brief Move-assigns another erased_ptr to this instance.
         *
         * Transfers ownership of the managed object and type information
         * from the given `erased_ptr` to this instance. The state of the
         * `other` erased_ptr is reset after the transfer.
         *
         * @param other The erased_ptr to move from.
         * @return A reference to the updated erased_ptr instance.
         */
        erased_ptr & operator=(erased_ptr && other) noexcept
        {
            data = std::move(other.data);
            type = other.type;
            other.type = typeid(void);
            return *this;
        }

        /**
         * @brief Constructs an erased_ptr object from a unique_ptr.
         *
         * Transfers ownership of the provided unique_ptr to a newly created
         * erased_ptr. The type information of the object is stored internally
         * and is used for runtime checks and safe type retrieval.
         *
         * @tparam T The type of the object managed by the unique_ptr.
         * @param ptr A unique_ptr managing the object to be stored. Ownership is
         *        transferred to the created erased_ptr object.
         *
         * @throws std::bad_alloc if memory allocation for the internal storage
         *         fails.
         */
        template <typename T>
        explicit erased_ptr(std::unique_ptr<T> && ptr)
            : data{ static_cast<void*>(ptr.release()), [](void * p) { delete static_cast<T*>(p); } }
            , type{ typeid(T) }
        { }

        /**
         * @brief Constructs an erased_ptr object from a raw pointer.
         *
         * Transfers ownership of the provided raw pointer to a newly created
         * erased_ptr. The type of the pointed-to object is preserved internally
         * for runtime checks and safe type retrieval.
         *
         * @tparam T The type of the object managed by the raw pointer.
         * @param ptr A raw pointer to the object to be stored. Ownership of the
         *        object is transferred to the erased_ptr, and it will be automatically
         *        deleted when the erased_ptr is destroyed or reset.
         *
         * @throws std::bad_alloc if memory allocation for the internal storage fails.
         */
        template <typename T>
        explicit erased_ptr(T * ptr)
            : data{ static_cast<void*>(ptr), [](void * p) { delete static_cast<T*>(p); } }
            , type{ typeid(T) }
        { }

        /**
         * @brief Retrieves a reference to the object stored in the erased_ptr.
         *
         * The method attempts to cast the stored object to the specified type T.
         * If the type of the stored object does not match the requested type T,
         * a std::bad_cast exception is thrown.
         *
         * @tparam T The type to which the stored object should be cast.
         * @return T& A reference to the stored object of type T.
         * @throws std::bad_cast if the type of the stored object does not match T.
         */
        template <typename T>
        T & get()
        {
            if (type != typeid(T))
            {
                throw std::bad_cast();
            }

            return *static_cast<T *>(data.get());
        }

        /**
         * @brief Retrieves a const reference to the object stored in the erased_ptr.
         *
         * The method attempts to cast the stored object to the specified type T.
         * If the type of the stored object does not match the requested type T,
         * a std::bad_cast exception is thrown.
         *
         * @tparam T The type to which the stored object should be cast.
         * @return const T& A reference to the stored object of type T.
         * @throws std::bad_cast if the type of the stored object does not match T.
         */
        template <typename T>
        const T & get() const
        {
            if (type != typeid(T))
            {
                throw std::bad_cast();
            }

            return *static_cast<const T *>(data.get());
        }

        /**
         * @brief Resets the erased_ptr to an empty state.
         *
         * Releases ownership of the currently managed object, if any, and resets
         * the stored type information to `void`. After this call, the erased_ptr
         * will no longer manage any object.
         *
         * @post The erased_ptr becomes empty and does not manage any resources.
         *       The stored type information is set to `void`.
         */
        inline void reset()
        {
            data.reset();
            type = typeid(void);
        }

        /**
         * @brief Resets the erased_ptr with a new object.
         *
         * Releases ownership of the currently managed object, if any, and replaces it
         * with the object pointed to by the provided raw pointer. Updates the stored
         * type information to the type of the new object.
         *
         * @tparam T The type of the object to be managed by the erased_ptr.
         * @param ptr A raw pointer to the new object. Ownership of the object
         *        is transferred to the erased_ptr, and it will manage the lifetime
         *        of the object, automatically deleting it when no longer needed.
         *
         * @post The erased_ptr manages the new object pointed to by `ptr`, and
         *       the stored type information is set to the type `T`.
         *
         * @warning Passing a null pointer will leave the erased_ptr in an empty state,
         *          with no object managed, and the stored type information reset to `void`.
         */
        template <typename T>
        void reset(T * ptr)
        {
            data.reset(static_cast<void*>(ptr), [](void * p) { delete static_cast<T*>(p); });
            type = typeid(T);
        }

    public:
        friend void swap(erased_ptr & lhs, erased_ptr & rhs ) noexcept;
    };

    /**
     * @brief Creates an erased_ptr that manages an instance of type T.
     *
     * Allocates a new instance of T using the provided arguments and wraps it
     * in an erased_ptr. This allows for type-erased handling of the instance
     * while maintaining proper lifetime management.
     *
     * @tparam T The type of the object to create.
     * @tparam Args The types of the arguments to forward to T's constructor.
     * @param args The arguments to be forwarded to the constructor of T.
     * @return An erased_ptr that manages the newly created instance of T.
     */
    template <typename T, typename... Args>
    erased_ptr make_erased(Args&&... args)
    {
        return erased_ptr(new T(std::forward<Args>(args)...));
    }

    /**
     * @brief Swaps the contents of two erased_ptr objects.
     *
     * Exchanges the managed resources and type information of the two specified
     * erased_ptr instances. After the swap operation, the content of `lhs` will
     * be in `rhs` and vice versa.
     *
     * @param lhs The first erased_ptr instance to swap.
     * @param rhs The second erased_ptr instance to swap.
     *
     * @post The `lhs` contains the resources and type information previously
     *       held by `rhs`, and `rhs` contains those previously held by `lhs`.
     */
    inline void swap(erased_ptr & lhs, erased_ptr & rhs ) noexcept
    {
        std::swap(lhs.data, rhs.data);
        std::swap(lhs.type, rhs.type);
    }

}