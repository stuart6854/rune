// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/utility/guid.hpp"

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

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
        Guid guid;
        std::string name;
        std::string tag;

        COMPONENT_DEFAULT_CTORS(EntityHeader)
    };

    struct Transform
    {
        COMPONENT_DEFAULT_CTORS(Transform)

        auto getPosition() const -> const glm::vec3&;
        void setPosition(const glm::vec3& position);

        auto getEulerAngles() const -> const glm::vec3&;
        void setEulerAngles(const glm::vec3& eulerAngles);

        auto getScale() const -> const glm::vec3&;
        void setScale(const glm::vec3& scale);

        auto getForward() const -> const glm::vec3&;
        auto getRight() const -> const glm::vec3&;
        auto getUp() const -> const glm::vec3&;

        auto getTransform() const -> const glm::mat4&;

    private:
        void updateDirections();
        void updateTransform();

    private:
        glm::vec3 m_position = { 0, 0, 0 };
        glm::vec3 m_eulerAngles = { 0, 0, 0 };
        glm::vec3 m_scale = { 1, 1, 1 };

        glm::vec3 m_forward = {};
        glm::vec3 m_right = {};
        glm::vec3 m_up = {};

        glm::mat4 m_transform = glm::mat4{ 1.0f };
    };

    struct MeshRenderer
    {
        Mesh* mesh;
        MaterialInst* material;

        COMPONENT_DEFAULT_CTORS(MeshRenderer)
    };

    struct ScriptBehaviour
    {
        /* eg. "Namespace.Class" */
        std::string classFullname;

        COMPONENT_DEFAULT_CTORS(ScriptBehaviour)
    };
}