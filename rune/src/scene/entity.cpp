// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "rune/scene/entity.hpp"

#include "rune/scene/components.hpp"

namespace Rune
{
    Entity::Entity() : m_scene(nullptr), m_entity(entt::null) {}

    Entity::Entity(Scene* scene, const entt::entity entity) : m_scene(scene), m_entity(entity) {}

    Entity::Entity(const Entity& entity) : m_scene(entity.m_scene), m_entity(entity.m_entity) {}

    auto Entity::getGuid() const -> const Guid&
    {
        return get<EntityHeader>()->guid;
    }

    auto Entity::getName() const -> const std::string&
    {
        return get<EntityHeader>()->name;
    }

    void Entity::setName(const std::string& name)
    {
        get<EntityHeader>()->name = name;
    }

    auto Entity::getTag() const -> const std::string&
    {
        return get<EntityHeader>()->tag;
    }

    void Entity::setTag(const std::string& tag)
    {
        get<EntityHeader>()->tag = tag;
    }

    Entity::operator u32() const
    {
        return static_cast<u32>(m_entity);
    }

    Entity::operator entt::entity() const
    {
        return m_entity;
    }
}
