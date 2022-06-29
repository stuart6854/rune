#include "rune/utility/buffer.hpp"

#include "rune/macros.hpp"

namespace Rune
{
    auto Buffer::copy(const void* data, const u32 size)
    {
        Buffer buffer;
        buffer.allocate(size);
        std::memcpy(buffer.getData(), data, size);
        return buffer;
    }

    Buffer::Buffer() : m_data(nullptr), m_size(0) {}

    Buffer::Buffer(void* data, const u32 size) : m_data(data), m_size(size) {}

    auto Buffer::getData() const -> void*
    {
        return m_data;
    }

    auto Buffer::getSize() const -> u32
    {
        return m_size;
    }

    void Buffer::allocate(const u32 size)
    {
        release();

        if (size == 0)
            return;

        m_data = new i8[size];
        m_size = size;
    }

    void Buffer::release()
    {
        delete[] m_data;
        m_data = nullptr;
        m_size = 0;
    }

    void Buffer::zeroInitialise() const
    {
        if (m_data)
        {
            std::memset(m_data, 0, m_size);
        }
    }

    auto Buffer::readBytes(const u32 size, const u32 offset) const -> u8*
    {
        RUNE_ENG_ASSERT(offset + size <= m_size, "Buffer overflow!");
        auto* buffer = new u8[size];
        std::memcpy(buffer, static_cast<u8*>(m_data) + offset, size);
        return buffer;
    }

    void Buffer::write(const void* data, const u32 size, const u32 offset) const
    {
        RUNE_ENG_ASSERT(offset + size <= m_size, "Buffer overflow!");
        std::memcpy(static_cast<u8*>(m_data) + offset, data, size);
    }

    Buffer::operator bool() const
    {
        return m_data;
    }
}
