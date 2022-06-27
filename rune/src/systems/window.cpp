#include "rune/systems/window.hpp"

#include "rune/init.hpp"
#include "rune/macros.hpp"

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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

        m_windowPtr = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        if (!m_windowPtr)
        {
            CORE_LOG_ERROR("[GLFW] Failed to create window!");
            return false;
        }
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_windowPtr));
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_windowPtr), &m_data);

        setVSync(true);

        glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(m_windowPtr), [](GLFWwindow* window) { Game::close(); });

        return true;
    }

    void WindowSystem::update() const
    {
        glfwSwapBuffers(static_cast<GLFWwindow*>(m_windowPtr));
        glfwPollEvents();
    }
}
