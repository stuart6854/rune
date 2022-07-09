#pragma once

#include "rune/defines.hpp"
#include "input_keys.hpp"

#include <array>

namespace Rune
{
    class InputSystem
    {
    public:
        static auto getInstance() -> InputSystem&;

        void init();
        void newFrame();
        void cleanup() const;

        bool isKeyDown(u32 key) const;
        bool isKeyUp(u32 key) const;
        bool isKeyHeld(u32 key) const;

        bool isMouseButtonDown(u32 btn) const;
        bool isMouseButtonUp(u32 btn) const;
        bool isMouseButtonHeld(u32 btn) const;

    private:
        void onKeyDown(u32 key);
        void onKeyUp(u32 key);

        void onMouseBtnDown(u32 btn);
        void onMouseBtnUp(u32 btn);

    private:
        u32 m_keyDownListener{};
        u32 m_keyUpListener{};

        u32 m_mouseBtnDownListener{};
        u32 m_mouseBtnUpListener{};

        struct State
        {
            std::array<bool, Input::KEY_LAST> keys;
            std::array<bool, Input::MOUSE_BUTTON_LAST> mouseButtons;
        };

        State m_currentState{};
        State m_lastState{};
    };
}
