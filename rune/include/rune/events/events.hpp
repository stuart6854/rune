// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"
#include "events_types.hpp"

#include <functional>
#include <map>

namespace Rune
{
    struct ICallbackVector
    {
        virtual ~ICallbackVector() = default;

        virtual void remove(const u32 id) = 0;
    };

    template <typename Event>
    struct CallbackVector final : ICallbackVector
    {
        using CallbackType = std::function<void(const Event&)>;
        using ContainerElement = std::pair<int, CallbackType>;
        using ContainerType = std::vector<ContainerElement>;
        ContainerType container;

        void remove(const u32 id) override
        {
            auto removeFrom =
                std::remove_if(container.begin(), container.end(), [id](const ContainerElement& element) { return element.first == id; });

            if (removeFrom != container.end())
                container.erase(removeFrom, container.end());
        }

        void add(const u32 id, const CallbackType& callback)
        {
            container.emplace_back(id, callback);
        }
    };

    class EventSystem
    {
    public:
        template <typename Event>
        static auto listen(const std::function<void(const Event&)>& callback) -> u32
        {
            const int id = s_nextId++;

            const auto eventTypeHash = typeid(Event).hash_code();
            auto& vector = s_callbacks[eventTypeHash];
            if (vector == nullptr)
            {
                vector.reset(new CallbackVector<Event>{});
            }

            auto* vectorImpl = static_cast<CallbackVector<Event>*>(vector.get());
            vectorImpl->add(id, callback);

            return id;
        }

        template <typename Event>
        static void unlisten(const u32 callbackId)
        {
            auto found = s_callbacks.find(typeid(Event).hash_code());
            if (found != s_callbacks.end())
                found->second->remove(callbackId);
        }

        template <typename Event>
        static void notify(const Event& event)
        {
            using CleanEventType = std::remove_const_t<Event>;

            using Vector = CallbackVector<CleanEventType>;
            auto found = s_callbacks.find(typeid(CleanEventType).hash_code());
            if (found == s_callbacks.end())
            {
                return;
            }

            std::unique_ptr<ICallbackVector>& vector = found->second;
            Vector* vectorImpl = static_cast<Vector*>(vector.get());

            for (auto& element : vectorImpl->container)
            {
                element.second(event);
            }
        }

    private:
        static std::map<size, Owned<ICallbackVector>> s_callbacks;
        static u32 s_nextId;
    };
}
