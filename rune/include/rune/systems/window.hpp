#pragma once

#include "rune/defines.hpp"

#include <string>

namespace Rune
{

    class WindowSystem
    {
    public:
        static auto getInstance() -> WindowSystem&;

        void init();
        void cleanup();

        struct WindowProps
        {
            std::string title = "Window";
            u32 width = 1280;
            u32 height = 768;
        };

        auto getTitle() const -> const std::string&;
        auto getWidth() const -> u32;
        auto getHeight() const -> u32;
        bool isVSync() const;

        void setTitle(const std::string& title);
        void setWidth(u32 value) const;
        void setHeight(u32 value) const;
        void setVSync(bool vSync);

        bool createWindow(const WindowProps& props = {});
        void update() const;

    private:
        struct WindowData
        {
            std::string title;
            u32 width;
            u32 height;
            bool vSync;
        };
        WindowData m_data;
        void* m_windowPtr;
    };
}
