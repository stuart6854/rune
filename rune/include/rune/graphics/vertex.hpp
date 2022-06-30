#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Rune
{
    struct Vertex
    {
        glm::vec3 pos{};
        glm::vec2 uv{};
        glm::vec3 norm{};
    };

}