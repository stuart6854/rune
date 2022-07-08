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

    private:
        void onKeyDown(u32 key);
        void onKeyUp(u32 key);

    private:
        u32 m_keyDownListener{};
        u32 m_keyUpListener{};

        struct State
        {
            std::array<bool, Input::KEY_LAST> keys;
        };

        State m_currentState{};
        State m_lastState{};
    };
}
