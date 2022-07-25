#include "pch.hpp"
#include "rune/scene/entity.hpp"

namespace Rune
{
    Entity::Entity() : m_scene(nullptr), m_entity(entt::null), m_guid(0) {}

    Entity::Entity(Scene* scene, const entt::entity entity) : m_scene(scene), m_entity(entity) {}

    Entity::Entity(const Entity& entity) : m_scene(entity.m_scene), m_entity(entity.m_entity), m_guid(entity.m_guid) {}

    auto Entity::getGuid() const -> const Guid&
    {
        return m_guid;
    }

    Entity::operator u64() const
    {
        return m_guid;
    }

    Entity::operator entt::entity() const
    {
        return m_entity;
    }
}
