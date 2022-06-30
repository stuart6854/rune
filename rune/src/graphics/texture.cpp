#include "rune/graphics/texture.hpp"

#include "rune/graphics/graphics.hpp"

namespace Rune
{
    Texture::~Texture()
    {
        onDestroying();
    }

    void Texture::attachObserver(Observer* observer)
    {
        m_observers.push_back(observer);
    }

    void Texture::detachObserver(Observer* observer)
    {
        const auto it = std::ranges::find(m_observers, observer);
        if (it != m_observers.end())
            m_observers.erase(it);
    }

    void Texture::init(const i32 width, const i32 height, const TextureFormat format, const std::vector<u8>& data)
    {
        m_width = width;
        m_height = height;
        m_format = format;

        m_data = data;
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

    void Texture::onDestroying() const
    {
        for (const auto& observer : m_observers)
        {
            observer->destroying(this);
        }
    }

    void Texture::onChanged() const
    {
        for (const auto& observer : m_observers)
        {
            observer->changed(this);
        }
    }
}
