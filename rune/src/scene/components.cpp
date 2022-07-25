// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "rune/scene/components.hpp"

#include <glm/gtx/quaternion.hpp>

namespace Rune
{
    auto Transform::getPosition() const -> const glm::vec3&
    {
        return m_position;
    }

    void Transform::setPosition(const glm::vec3& position)
    {
        m_position = position;
        updateTransform();
    }

    auto Transform::getEulerAngles() const -> const glm::vec3&
    {
        return m_eulerAngles;
    }

    void Transform::setEulerAngles(const glm::vec3& eulerAngles)
    {
        m_eulerAngles = eulerAngles;
        updateDirections();
        updateTransform();
    }

    auto Transform::getScale() const -> const glm::vec3&
    {
        return m_scale;
    }

    void Transform::setScale(const glm::vec3& scale)
    {
        m_scale = scale;
        updateTransform();
    }

    auto Transform::getForward() const -> const glm::vec3&
    {
        return m_forward;
    }

    auto Transform::getRight() const -> const glm::vec3&
    {
        return m_right;
    }

    auto Transform::getUp() const -> const glm::vec3&
    {
        return m_up;
    }

    auto Transform::getTransform() const -> const glm::mat4&
    {
        return m_transform;
    }

    void Transform::updateDirections()
    {
        glm::vec3 rotRadians = glm::radians(m_eulerAngles);
        m_forward = glm::rotate(glm::quat(rotRadians), { 0, 0, 1 });
        m_right = glm::cross(glm::vec3{ 0, 1, 0 }, m_forward);
        m_up = glm::cross(m_forward, m_right);
    }

    void Transform::updateTransform()
    {
        m_transform = glm::translate(glm::mat4(1.0f), m_position);
        m_transform = glm::rotate(m_transform, glm::radians(m_eulerAngles.z), { 0, 0, 1 });
        m_transform = glm::rotate(m_transform, glm::radians(m_eulerAngles.y), { 0, 1, 0 });
        m_transform = glm::rotate(m_transform, glm::radians(m_eulerAngles.x), { 1, 0, 0 });
        m_transform = glm::scale(m_transform, m_scale);
    }
}
