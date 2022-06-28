#pragma once

#include "rune/defines.hpp"

#include <vector>
#include <queue>

namespace Rune
{
    template <typename T>
    class Storage
    {
    public:
        auto add(const T& value) -> u32;

        auto get(u32 index) -> T&;

        void remove(u32 index);

        auto count() const -> u32;

    private:
        std::vector<T> m_storage;
        std::queue<u32> m_unusedIndices;
        i32 m_count = 0;
    };

    template <typename T>
    auto Storage<T>::add(const T& value) -> u32
    {
        u32 index;
        // Check for unused index
        if (!m_unusedIndices.empty())
        {
            index = m_unusedIndices.front();
            m_unusedIndices.pop();

            // Add the value to the existing space
            m_storage[index] = value;
        }
        else
        {
            // Push the value into the vector and get it index
            index = m_storage.size();
            m_storage.push_back(value);
        }

        ++m_count;

        return index + 1;
    }

    template <typename T>
    auto Storage<T>::get(const u32 index) -> T&
    {
        return m_storage[index - 1];
    }

    template <typename T>
    void Storage<T>::remove(const u32 index)
    {
        m_unusedIndices.push(index - 1);
        --m_count;
    }

    template <typename T>
    auto Storage<T>::count() const -> u32
    {
        return m_count;
    }
}
