// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"

#include <glm/ext/matrix_float4x4.hpp>

namespace Rune
{
    class Material;
    class Mesh;

    class SceneRenderer
    {
    public:
        void beginScene(const glm::mat4& proj, const glm::mat4& view);
        void endScene();

        void submit(Shared<Mesh> mesh, Shared<Material> material, const glm::mat4& transform = glm::mat4(1.0f));

    private:
        void flushDrawList();

    private:
        struct SceneCamera
        {
            glm::mat4 proj = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);

            glm::vec3 position = { 0, 0, 0 };
        } m_sceneCamera;

        struct DrawItem
        {
            Shared<Mesh> mesh;
            Shared<Material> material;
            glm::mat4 transform = glm::mat4(1.0f);
        };

        std::vector<DrawItem> m_drawList;
    };
}
