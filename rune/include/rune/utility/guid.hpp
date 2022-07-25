// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"

namespace Rune
{
    class Guid
    {
    public:
        Guid();
        Guid(u64 guid);
        Guid(const Guid& other) = default;

        operator u64() const;

    private:
        u64 m_guid;
    };
}

namespace std
{
    template <>
    struct hash<Rune::Guid>
    {
        auto operator()(const Rune::Guid& guid) const noexcept -> std::size_t
        {
            return hash<u64>()(guid);
        }
    };
}