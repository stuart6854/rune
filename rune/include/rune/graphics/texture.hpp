// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/assets/asset.hpp"

#include <vector>

namespace Rune
{
    enum class TextureFormat : u8
    {
        eUnknown,
        eR,
        eRGB,
        eRGBA,
    };

    class Texture : public Asset
    {
    public:
        ~Texture() override;

        void init(i32 width, i32 height, TextureFormat format, const std::vector<u8>& data);

        auto getWidth() const -> i32;
        auto getHeight() const -> i32;
        auto getFormat() const -> TextureFormat;

        auto getData() const -> const std::vector<u8>&;

        auto getInternalId() const -> u32;

    private:
        u32 m_internalId{};

        i32 m_width{};
        i32 m_height{};
        TextureFormat m_format{};

        std::vector<u8> m_data;
    };
}
