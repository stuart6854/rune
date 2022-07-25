// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"

#include <vector>
#include <functional>

namespace Rune
{
    template <typename... Args>
    class Callback
    {
        using Func = std::function<void(Args...)>;

    public:
        auto addListener(Func func) -> size
        {
            auto id = m_nextId++;
            m_callbacks.emplace_back(func, id);
            return id;
        }

        void removeListener(size id)
        {
            for (auto it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
            {
                if ((*it).id == id)
                {
                    m_callbacks.erase(it);
                    return;
                }
            }
        }

        void invoke(Args... args)
        {
            for (const auto& callback : m_callbacks)
            {
                callback.func(args...);
            }
        }

    private:
        struct CallbackStorage
        {
            Func func;
            size id;
        };

        std::vector<CallbackStorage> m_callbacks;
        size m_nextId{};
    };
}