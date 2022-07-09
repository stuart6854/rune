#include "rune/core/window.hpp"

#include "rune/init.hpp"
#include "rune/macros.hpp"
#include "rune/events/events.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Rune
{
    static bool s_glfwReady = false;

    static void glfwError(int e, const char* msg)
    {
        CORE_LOG_ERROR("[GLFW] {}: {}", e, msg);
    }

    auto WindowSystem::getInstance() -> WindowSystem&
    {
        static WindowSystem windowSystem;
        return windowSystem;
    }

    void WindowSystem::init()
    {
        if (s_glfwReady)
            return;

        if (!glfwInit())
        {
            CORE_LOG_ERROR("[Window] Failed to initialise GLFW!");
        }

        glfwSetErrorCallback(glfwError);

        s_glfwReady = true;
    }

    void WindowSystem::cleanup()
    {
        glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(m_windowPtr));
        glfwTerminate();
    }

    auto WindowSystem::getTitle() const -> const std::string&
    {
        return m_data.title;
    }

    auto WindowSystem::getWidth() const -> u32
    {
        return m_data.width;
    }

    auto WindowSystem::getHeight() const -> u32
    {
        return m_data.height;
    }

    bool WindowSystem::isVSync() const
    {
        return m_data.vSync;
    }

    auto WindowSystem::getWindowMode() const -> WindowMode
    {
        return m_data.mode;
    }

    void WindowSystem::setTitle(const std::string& title)
    {
        m_data.title = title;
        glfwSetWindowTitle(static_cast<GLFWwindow*>(m_windowPtr), title.c_str());
    }

    void WindowSystem::setWidth(const u32 value) const
    {
        glfwSetWindowSize(static_cast<GLFWwindow*>(m_windowPtr), value, m_data.height);
    }

    void WindowSystem::setHeight(const u32 value) const
    {
        glfwSetWindowSize(static_cast<GLFWwindow*>(m_windowPtr), m_data.width, value);
    }

    void WindowSystem::setVSync(const bool vSync)
    {
        glfwSwapInterval(vSync ? 1 : 0);
        m_data.vSync = vSync;
    }

    void WindowSystem::setWindowMode(const WindowMode mode)
    {
        if (mode == m_data.mode)
            return;

        auto* glfwWindow = static_cast<GLFWwindow*>(m_windowPtr);

        m_data.mode = mode;
        if (mode == WindowMode::eWindowed)
        {
            glfwSetWindowMonitor(glfwWindow, nullptr, 0, 0, m_data.width, m_data.height, GLFW_DONT_CARE);

            CORE_LOG_TRACE("Windowed Window: {}x{}", m_data.width, m_data.height);
        }
        else if (mode == WindowMode::eFullscreen)
        {
            glfwSetWindowMonitor(glfwWindow, glfwGetPrimaryMonitor(), 0, 0, m_data.width, m_data.height, GLFW_DONT_CARE);
            CORE_LOG_TRACE("Fullscreen Window: {}x{}", m_data.width, m_data.height);
        }
        else if (mode == WindowMode::eBorderless)
        {
            auto* monitor = glfwGetPrimaryMonitor();
            auto* videoMode = glfwGetVideoMode(monitor);

            m_data.width = videoMode->width;
            m_data.height = videoMode->height;

            glfwSetWindowMonitor(glfwWindow, monitor, 0, 0, m_data.width, m_data.height, videoMode->refreshRate);

            EventSystem::notify(EventFramebufferSize{ m_data.width, m_data.height });

            CORE_LOG_TRACE("Borderless Window: {}x{}x{}hz", m_data.width, m_data.height, videoMode->refreshRate);
        }
    }

    bool WindowSystem::createWindow(const WindowProps& props)
    {
        if (!s_glfwReady)
        {
            CORE_LOG_ERROR("[GLFW] GLFW it not initialised!");
            return false;
        }

        m_data.title = props.title;
        m_data.width = props.width;
        m_data.height = props.height;

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);

        m_windowPtr = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_windowPtr)
        {
            CORE_LOG_ERROR("[GLFW] Failed to create window!");
            return false;
        }
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_windowPtr));
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_windowPtr), &m_data);

        setWindowMode(props.windowMode);
        setVSync(props.vSync);

        glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(m_windowPtr),
                                   [](GLFWwindow* window) { EventSystem::notify(EventWindowClose{}); });

        glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(m_windowPtr),
                                  [](GLFWwindow* window, const int width, const int height)
                                  {
                                      /*auto* windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                                      windowData->width = width;
                                      windowData->height = height;*/
                                      // CORE_LOG_TRACE("Window Size: {}, {}", width, height);

                                      EventSystem::notify(EventWindowSize{ width, height });
                                  });

        glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(m_windowPtr),
                                       [](GLFWwindow* window, const int width, const int height)
                                       {
                                           auto* windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                                           windowData->width = width;
                                           windowData->height = height;
                                           // CORE_LOG_TRACE("Window Framebuffer: {}, {}", width, height);

                                           EventSystem::notify(EventFramebufferSize{ width, height });
                                       });

        glfwSetKeyCallback(static_cast<GLFWwindow*>(m_windowPtr),
                           [](GLFWwindow* window, const int key, int, const int action, int)
                           {
                               if (action == GLFW_PRESS)
                                   EventSystem::notify(EventKeyDown{ key });
                               else if (action == GLFW_RELEASE)
                                   EventSystem::notify(EventKeyUp{ key });
                           });

        glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(m_windowPtr),
                                   [](GLFWwindow* window, const int btn, const int action, int)
                                   {
                                       if (action == GLFW_PRESS)
                                           EventSystem::notify(EventMouseBtnDown{ btn });
                                       else if (action == GLFW_RELEASE)
                                           EventSystem::notify(EventMouseBtnUp{ btn });
                                   });

        glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_windowPtr),
                                 [](GLFWwindow* window, const double x, const double y) {
                                     EventSystem::notify(EventCursorPos{ x, y });
                                 });

        return true;
    }

    void WindowSystem::update() const
    {
        glfwSwapBuffers(static_cast<GLFWwindow*>(m_windowPtr));
        glfwPollEvents();
    }

}
