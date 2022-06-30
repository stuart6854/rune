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
        class Observer
        {
        public:
            virtual ~Observer() = default;

            virtual void destroying(const Texture* texture) = 0;
            virtual void changed(const Texture* texture) = 0;
        };

    public:
        ~Texture() override;

        void attachObserver(Observer* observer);
        void detachObserver(Observer* observer);

        void init(i32 width, i32 height, TextureFormat format, const std::vector<u8>& data);

        auto getWidth() const -> i32;
        auto getHeight() const -> i32;
        auto getFormat() const -> TextureFormat;

        auto getData() const -> const std::vector<u8>&;

    protected:
        void onDestroying() const;
        void onChanged() const;

    private:
        std::vector<Observer*> m_observers;

        i32 m_width{};
        i32 m_height{};
        TextureFormat m_format{};

        std::vector<u8> m_data;
    };
}
