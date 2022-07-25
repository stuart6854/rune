#include "pch.hpp"
#include "rune/scene/entity.hpp"

#include "rune/scene/components.hpp"

namespace Rune
{
    Entity::Entity() : m_scene(nullptr), m_entity(entt::null), m_guid(0) {}

    Entity::Entity(Scene* scene, const entt::entity entity) : m_scene(scene), m_entity(entity) {}

    Entity::Entity(const Entity& entity) : m_scene(entity.m_scene), m_entity(entity.m_entity), m_guid(entity.m_guid) {}

    auto Entity::getGuid() const -> const Guid&
    {
        return m_guid;
    }

    auto Entity::getName() -> const std::string&
    {
        return get<EntityHeader>()->name;
    }

    void Entity::setName(const std::string& name)
    {
        get<EntityHeader>()->name = name;
    }

    auto Entity::getTag() -> const std::string&
    {
        return get<EntityHeader>()->tag;
    }

    void Entity::setTag(const std::string& tag)
    {
        get<EntityHeader>()->tag = tag;
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
