#pragma once

#include <queue>
#include <map>

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
        std::map<E, std::vector<Subscriber<E>*>> _subscribers;
        std::mutex                               _subscribersMutex;

        std::queue<Message<E>>                   _queue;
        std::mutex                               _queueMutex;

    public:
        /**
         * Queues a new message for publishing.
         *
         * This method adds a message to the publishing queue in a thread-safe manner.
         *
         * @tparam E The enumeration or type representing the message type.
         * @param message The message to be added to the queue.
         */
        void publish(const Message<E> & message)
        {
            std::lock_guard lock(_queueMutex);
            _queue.push(message);
        }

        /**
         * Subscribes a given subscriber to a specific type of message.
         *
         * This method registers a subscriber to receive notifications whenever a
         * message of the specified type is published. If the subscriber is already
         * registered to that type, no duplicate entry is added. The method is thread-safe.
         *
         * @tparam E The enumeration or type representing the message type.
         * @param subscriber Reference to the subscriber to be registered.
         * @param type The message type to which the subscriber should subscribe.
         */
        void subscribe(Subscriber<E> & subscriber, E type)
        {
            std::lock_guard lock(_subscribersMutex);

            auto group = _subscribers.find(type);
            if (group != _subscribers.end())
            {
                if (std::find(group->second.begin(), group->second.end(), &subscriber) == group->second.end())
                {
                    group.value().push_back( &subscriber );
                }
            }
            else
            {
                _subscribers.insert({ type, std::vector<Subscriber<E>*>{ &subscriber } });
            }
        }

        /**
         * Unsubscribes a given subscriber from a specific type of message.
         *
         * This method removes a subscriber from the list of subscribers associated
         * with the specified message type. If the subscriber is not registered for
         * the type, the method makes no changes. The method is thread-safe.
         *
         * @tparam E The enumeration or type representing the message type.
         * @param subscriber Reference to the subscriber to be unregistered.
         * @param type The message type from which the subscriber should be removed.
         */
        void unsubscribe(Subscriber<E> & subscriber, E type)
        {
            std::lock_guard lock(_subscribersMutex);

            auto group = _subscribers.find(type);
            if (group != _subscribers.end())
            {
                auto it = std::find(group->second.begin(), group->second.end(), &subscriber);
                if (it != group->second.end())
                {
                    group->second.erase(it);
                }
            }
        }

        /**
         * Unsubscribes a given subscriber from all message types to which it is currently subscribed.
         *
         * This method removes a subscriber from all subscription lists across all message types.
         * If the subscriber is not registered for any type, the method makes no changes. The method
         * is thread-safe.
         *
         * @tparam E The enumeration or type representing the message type.
         * @param subscriber Reference to the subscriber to be unregistered from all message types.
         */
        void unsubscribe(Subscriber<E> & subscriber)
        {
            std::lock_guard lock(_subscribersMutex);

            for (auto & group : _subscribers)
            {
                auto it = std::find(group.second.begin(), group.second.end(), &subscriber);
                if (it != group.second.end())
                {
                    group.second.erase(it);
                }
            }
        }

        // TODO thread with condition variable to handle queue on publish
    };
}
