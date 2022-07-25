// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"

namespace Rune
{
    class Scene;

    class SceneManager
    {
    public:
        static auto getInstance() -> SceneManager&;

        void init();
        void cleanup() const;
        void update() const;

        void loadScene(const std::string& sceneName);

        auto getActiveScene() const -> Scene*;

    private:
        Owned<Scene> m_activeScene;
    };
}
