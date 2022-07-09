#include "rune/input/input.hpp"

#include "rune/events/events.hpp"

namespace Rune
{
    auto InputSystem::getInstance() -> InputSystem&
    {
        static InputSystem input;
        return input;
    }

    void InputSystem::init()
    {
        m_currentState = {};

        // Bind events
        m_keyDownListener = EventSystem::listen<EventKeyDown>([this](const EventKeyDown& event) { this->onKeyDown(event.key); });
        m_keyUpListener = EventSystem::listen<EventKeyUp>([this](const EventKeyUp& event) { this->onKeyUp(event.key); });

        m_mouseBtnDownListener =
            EventSystem::listen<EventMouseBtnDown>([this](const EventMouseBtnDown& event) { this->onMouseBtnDown(event.btn); });
        m_mouseBtnUpListener =
            EventSystem::listen<EventMouseBtnUp>([this](const EventMouseBtnUp& event) { this->onMouseBtnUp(event.btn); });
    }

    void InputSystem::newFrame()
    {
        m_lastState = m_currentState;
    }

    void InputSystem::cleanup() const
    {
        EventSystem::unlisten<EventKeyDown>(m_keyDownListener);
        EventSystem::unlisten<EventKeyUp>(m_keyUpListener);

        EventSystem::unlisten<EventMouseBtnDown>(m_mouseBtnDownListener);
        EventSystem::unlisten<EventMouseBtnUp>(m_mouseBtnUpListener);
    }

    bool InputSystem::isKeyDown(const u32 key) const
    {
        return m_currentState.keys[key] && !m_lastState.keys[key];
    }

    bool InputSystem::isKeyUp(const u32 key) const
    {
        return !m_currentState.keys[key] && m_lastState.keys[key];
    }

    bool InputSystem::isKeyHeld(const u32 key) const
    {
        return m_currentState.keys[key] && m_lastState.keys[key];
    }

    bool InputSystem::isMouseButtonDown(const u32 btn) const
    {
        return m_currentState.mouseButtons[btn] && !m_lastState.mouseButtons[btn];
    }

    bool InputSystem::isMouseButtonUp(const u32 btn) const
    {
        return !m_currentState.mouseButtons[btn] && m_lastState.mouseButtons[btn];
    }

    bool InputSystem::isMouseButtonHeld(const u32 btn) const
    {
        return m_currentState.mouseButtons[btn] && m_lastState.mouseButtons[btn];
    }

    void InputSystem::onKeyDown(const u32 key)
    {
        m_currentState.keys[key] = true;
    }

    void InputSystem::onKeyUp(const u32 key)
    {
        m_currentState.keys[key] = false;
    }

    void InputSystem::onMouseBtnDown(const u32 btn)
    {
        m_currentState.mouseButtons[btn] = true;
    }

    void InputSystem::onMouseBtnUp(const u32 btn)
    {
        m_currentState.mouseButtons[btn] = false;
    }
}
