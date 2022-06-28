#pragma once

#include "rune/systems/assets/asset.hpp"

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

        void init(i32 width, i32 height, TextureFormat format, const void* data);

    private:
        i32 m_width{};
        i32 m_height{};
        TextureFormat m_format{};

        i32 m_internalTexture{};
    };
}
