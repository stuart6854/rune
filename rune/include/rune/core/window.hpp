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

    enum class CursorMode : u8
    {
        eNormal = 0,
        eDisabled,
        eHidden
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
            i32 width = 1280;
            i32 height = 768;
            bool vSync = true;
            WindowMode windowMode = WindowMode::eWindowed;
        };

        auto getTitle() const -> const std::string&;
        auto getWidth() const -> u32;
        auto getHeight() const -> u32;
        bool isVSync() const;
        auto getWindowMode() const -> WindowMode;
        auto getCursorMode() const -> CursorMode;

        void setTitle(const std::string& title);
        void setWidth(u32 value) const;
        void setHeight(u32 value) const;
        void setVSync(bool vSync);
        void setWindowMode(WindowMode mode);
        void setCursorMode(CursorMode mode);

        bool createWindow(const WindowProps& props = {});
        void update() const;

    private:
        struct WindowData
        {
            std::string title;
            i32 width;
            i32 height;
            bool vSync;
            WindowMode windowMode;
            CursorMode cursorMode;
        };
        WindowData m_data;
        void* m_windowPtr;
    };
}
