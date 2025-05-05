#pragma once

#include "erased.hpp"

namespace light
{
    template <typename E>
    struct Message
    {
        E type;
        erased_ptr data;
    };

    template <typename E>
    class Subscriber
    {
    public:
        virtual ~Subscriber() = default;
        virtual bool onReceive(const Message<E> & message) = 0;
    };

    template <typename E>
    class Broker
    {
    public:
        virtual ~Broker() = default;
        virtual void publish(const Message<E> & message) = 0;
        virtual void subscribe(Subscriber<E> & subscriber) = 0;
    };
}
