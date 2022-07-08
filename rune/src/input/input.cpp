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
    }

    void InputSystem::newFrame()
    {
        m_lastState = m_currentState;
    }

    void InputSystem::cleanup() const
    {
        EventSystem::unlisten<EventKeyDown>(m_keyDownListener);
        EventSystem::unlisten<EventKeyUp>(m_keyUpListener);
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

    void InputSystem::onKeyDown(const u32 key)
    {
        m_currentState.keys[key] = true;
    }

    void InputSystem::onKeyUp(const u32 key)
    {
        m_currentState.keys[key] = false;
    }
}
