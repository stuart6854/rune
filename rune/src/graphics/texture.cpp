#include "rune/graphics/texture.hpp"

#include "rune/graphics/graphics.hpp"

namespace Rune
{
    Texture::~Texture()
    {
        auto& graphicsInst = GraphicsSystem::getInstance();
        graphicsInst.destroyTexture(m_internalTexture);
    }

    void Texture::init(const i32 width, const i32 height, const TextureFormat format, const void* data)
    {
        m_width = width;
        m_height = height;
        m_format = format;

        auto& graphicsInst = GraphicsSystem::getInstance();
        m_internalTexture = graphicsInst.createTexture(m_width, m_height, m_format, data);
    }
}
