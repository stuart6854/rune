#include "rune/systems/assets/asset_factory.hpp"

#include "rune/macros.hpp"
#include "rune/systems/graphics/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Rune
{
    auto formatFromChannels(const i32 c) -> TextureFormat
    {
        if (c == 1)
            return TextureFormat::eR;
        if (c == 3)
            return TextureFormat::eRGB;
        if (c == 4)
            return TextureFormat::eRGBA;

        return TextureFormat::eUnknown;
    }

    auto TextureFactory::createFromFile(const std::string& filename) -> Owned<Asset>
    {
        // Create texture
        auto texture = CreateOwned<Texture>();

        // Enable flipping texture on load
        stbi_set_flip_vertically_on_load(true);

        // Load texture file
        i32 w, h, c;
        auto* data = stbi_load(filename.c_str(), &w, &h, &c, 0);
        if (data == nullptr)
        {
            CORE_LOG_ERROR("Failed to load texture file: {}", filename);
            return nullptr;
        }

        // Get texture format from # of channels
        TextureFormat format = formatFromChannels(c);
        if (format == TextureFormat::eUnknown)
        {
            CORE_LOG_ERROR("Trying to load unsupported texture format! channels={}", c);
            stbi_image_free(data);
            return nullptr;
        }

        // Init texture with loaded data
        texture->init(w, h, format, data);

        // Free texture data
        stbi_image_free(data);

        return std::move(texture);
    }
}
