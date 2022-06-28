#pragma once

#include "rune/defines.hpp"

#include <string>

namespace Rune
{
    enum class WindowMode : u8
    {
        eWindowed = 0,
        eFullscreen,
        eBorderless
    };

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
            WindowMode windowMode = WindowMode::eWindowed;
        };

        auto getTitle() const -> const std::string&;
        auto getWidth() const -> u32;
        auto getHeight() const -> u32;
        bool isVSync() const;
        auto getWindowMode() const -> WindowMode;

        void setTitle(const std::string& title);
        void setWidth(u32 value) const;
        void setHeight(u32 value) const;
        void setVSync(bool vSync);
        void setWindowMode(WindowMode mode);

        bool createWindow(const WindowProps& props = {});
        void update() const;

    private:
        struct WindowData
        {
            std::string title;
            u32 width;
            u32 height;
            bool vSync;
            WindowMode mode;
        };
        WindowData m_data;
        void* m_windowPtr;
    };
}
