#include "pch.hpp"
#include "rune/utility/guid.hpp"

#include "rune/maths/random.hpp"

namespace Rune
{
    Guid::Guid() : m_guid(Random::randomLong()) {}

    Guid::Guid(const u64 guid) : m_guid(guid) {}

    Guid::operator u64() const
    {
        return m_guid;
    }
}
