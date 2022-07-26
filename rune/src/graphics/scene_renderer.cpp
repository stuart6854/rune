// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "scene_renderer.hpp"

namespace Rune
{
    void SceneRenderer::beginScene(const glm::mat4& proj, const glm::mat4& view)
    {
        m_sceneCamera.proj = proj;
        m_sceneCamera.view = view;

        m_sceneCamera.position = glm::inverse(view)[3];
    }

    void SceneRenderer::endScene()
    {
        flushDrawList();
    }

    void SceneRenderer::submit(Shared<Mesh> mesh, Shared<Material> material, const glm::mat4& transform)
    {
        DrawItem drawItem{
            .mesh = mesh,
            .material = material,
            .transform = transform,
        };
        m_drawList.push_back(drawItem);
    }

    void SceneRenderer::flushDrawList()
    {
        m_drawList.clear();
    }
}
