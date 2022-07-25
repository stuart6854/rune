// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"
#include "input_keys.hpp"

#include <glm/vec2.hpp>

#include <array>

namespace Rune
{
    class InputSystem
    {
    public:
        static auto getInstance() -> InputSystem&;

        void init(const glm::vec2& windowSize);
        void newFrame();
        void cleanup() const;

        bool isKeyDown(u32 key) const;
        bool isKeyUp(u32 key) const;
        bool isKeyHeld(u32 key) const;

        bool isMouseButtonDown(u32 btn) const;
        bool isMouseButtonUp(u32 btn) const;
        bool isMouseButtonHeld(u32 btn) const;

        auto getCursorPos() const -> const glm::vec2&;
        auto getCursorDelta() const -> glm::vec2;

    private:
        void onWindowSize(double w, double h);

        void onKeyDown(u32 key);
        void onKeyUp(u32 key);

        void onMouseBtnDown(u32 btn);
        void onMouseBtnUp(u32 btn);

        void onCursorPos(double x, double y);

    private:
        u32 m_windowSizeListener{};

        u32 m_keyDownListener{};
        u32 m_keyUpListener{};

        u32 m_mouseBtnDownListener{};
        u32 m_mouseBtnUpListener{};

        u32 m_cursorPosListener{};

        glm::vec2 m_windowSize;

        struct State
        {
            std::array<bool, Input::KEY_LAST> keys;
            std::array<bool, Input::MOUSE_BUTTON_LAST> mouseButtons;
            glm::vec2 cursorPos = { 0, 0 };
        };

        State m_currentState{};
        State m_lastState{};
    };
}
