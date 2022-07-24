#include "pch.hpp"
#include "rune/graphics/texture.hpp"

#include "rune/graphics/graphics.hpp"

namespace Rune
{
    Texture::~Texture()
    {
        auto* renderer = GraphicsSystem::getInstance().getRenderer();
        renderer->destroyTexture(m_internalId);
    }

    void Texture::init(const i32 width, const i32 height, const TextureFormat format, const std::vector<u8>& data)
    {
        m_width = width;
        m_height = height;
        m_format = format;

        m_data = data;

        auto* renderer = GraphicsSystem::getInstance().getRenderer();
        m_internalId = renderer->createTexture(m_width, m_height, m_format, m_data.data());
    }

    auto Texture::getWidth() const -> i32
    {
        return m_width;
    }

    auto Texture::getHeight() const -> i32
    {
        return m_height;
    }

    auto Texture::getFormat() const -> TextureFormat
    {
        return m_format;
    }

    auto Texture::getData() const -> const std::vector<u8>&
    {
        return m_data;
    }

    auto Texture::getInternalId() const -> u32
    {
        return m_internalId;
    }
}
