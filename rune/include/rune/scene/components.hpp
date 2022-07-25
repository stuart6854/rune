// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include <glm/ext/vector_float3.hpp>

#include <string>

#define COMPONENT_DEFAULT_CTORS(COMP_NAME) \
    COMP_NAME() = default;                 \
    COMP_NAME(const COMP_NAME&) = default;

namespace Rune
{
    class MaterialInst;
    class Mesh;

    struct EntityHeader
    {
        std::string name;
        std::string tag;

        COMPONENT_DEFAULT_CTORS(EntityHeader)
    };

    struct Transform
    {
        glm::vec3 position = { 0, 0, 0 };
        glm::vec3 rotation = { 0, 0, 0 };
        glm::vec3 scale = { 1, 1, 1 };

        COMPONENT_DEFAULT_CTORS(Transform)
    };

    struct MeshRenderer
    {
        Mesh* mesh;
        MaterialInst* material;

        COMPONENT_DEFAULT_CTORS(MeshRenderer)
    };
}