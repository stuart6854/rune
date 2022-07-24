#include "pch.hpp"
#include "rune/input/input.hpp"

#include "glm/common.hpp"
#include "glm/common.hpp"
#include "rune/events/events.hpp"

namespace Rune
{
    auto InputSystem::getInstance() -> InputSystem&
    {
        static InputSystem input;
        return input;
    }

    void InputSystem::init(const glm::vec2& windowSize)
    {
        m_windowSize = windowSize;

        m_currentState = {};

        // Bind events
        m_windowSizeListener =
            EventSystem::listen<EventWindowSize>([this](const EventWindowSize& event) { this->onWindowSize(event.width, event.height); });

        m_keyDownListener = EventSystem::listen<EventKeyDown>([this](const EventKeyDown& event) { this->onKeyDown(event.key); });
        m_keyUpListener = EventSystem::listen<EventKeyUp>([this](const EventKeyUp& event) { this->onKeyUp(event.key); });

        m_mouseBtnDownListener =
            EventSystem::listen<EventMouseBtnDown>([this](const EventMouseBtnDown& event) { this->onMouseBtnDown(event.btn); });
        m_mouseBtnUpListener =
            EventSystem::listen<EventMouseBtnUp>([this](const EventMouseBtnUp& event) { this->onMouseBtnUp(event.btn); });

        m_cursorPosListener =
            EventSystem::listen<EventCursorPos>([this](const EventCursorPos& event) { this->onCursorPos(event.x, event.y); });
    }

    void InputSystem::newFrame()
    {
        m_lastState = m_currentState;
    }

    void InputSystem::cleanup() const
    {
        EventSystem::unlisten<EventWindowSize>(m_windowSizeListener);

        EventSystem::unlisten<EventKeyDown>(m_keyDownListener);
        EventSystem::unlisten<EventKeyUp>(m_keyUpListener);

        EventSystem::unlisten<EventMouseBtnDown>(m_mouseBtnDownListener);
        EventSystem::unlisten<EventMouseBtnUp>(m_mouseBtnUpListener);

        EventSystem::unlisten<EventCursorPos>(m_cursorPosListener);
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

    auto InputSystem::getCursorPos() const -> const glm::vec2&
    {
        return m_currentState.cursorPos;
    }

    auto InputSystem::getCursorDelta() const -> glm::vec2
    {
        return { (m_currentState.cursorPos.x - m_lastState.cursorPos.x) / m_windowSize.x,
                 -(m_currentState.cursorPos.y - m_lastState.cursorPos.y) / m_windowSize.y };
    }

    void InputSystem::onWindowSize(double w, double h)
    {
        m_windowSize = { w, h };
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

    void InputSystem::onCursorPos(double x, double y)
    {
        m_currentState.cursorPos = { x, y };
    }
}
