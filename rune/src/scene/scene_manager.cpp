// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "rune/scene/scene_manager.hpp"

#include "rune/macros.hpp"
#include "rune/scene/scene.hpp"

namespace Rune
{
    auto SceneManager::getInstance() -> SceneManager&
    {
        static SceneManager sceneManager;
        return sceneManager;
    }

    void SceneManager::init()
    {
        // Create empty scene
        m_activeScene = CreateOwned<Scene>();
        m_activeScene->init();
    }

    void SceneManager::cleanup() const
    {
        RUNE_ENG_ASSERT(m_activeScene, "m_activeScene should not be nullptr!");
        m_activeScene->cleanup();
    }

    void SceneManager::update() const
    {
        RUNE_ENG_ASSERT(m_activeScene, "m_activeScene should not be nullptr!");
        m_activeScene->update();
    }

    void SceneManager::loadScene(const std::string& sceneName)
    {
        RUNE_ENG_ASSERT(m_activeScene, "m_activeScene should not be nullptr!");
        m_activeScene->cleanup();
        m_activeScene = CreateOwned<Scene>();
        // TODO: Load scene asset file
        m_activeScene->init();
    }

    auto SceneManager::getActiveScene() const -> Scene*
    {
        return m_activeScene.get();
    }
}
