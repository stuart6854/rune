#pragma once

#include "rune/defines.hpp"

namespace Rune
{
    class Buffer
    {
    public:
        static auto copy(const void* data, u32 size);

        Buffer();
        Buffer(void* data, u32 size);

        auto getData() const -> void*;
        auto getSize() const -> u32;

        void allocate(u32 size);
        void release();

        void zeroInitialise() const;

        auto readBytes(u32 size, u32 offset) const -> u8*;
        void write(const void* data, u32 size, u32 offset = 0) const;

        operator bool() const;

    private:
        void* m_data;
        u32 m_size;
    };
}