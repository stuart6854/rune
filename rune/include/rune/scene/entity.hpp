// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "scene.hpp"
#include "rune/defines.hpp"
#include "rune/utility/guid.hpp"

#include <entt/entity/entity.hpp>

namespace Rune
{
    class Entity
    {
    public:
        Entity();
        Entity(Scene* scene, entt::entity entity);
        Entity(const Entity&);

        auto getGuid() const -> const Guid&;

        auto getName() const -> const std::string&;
        void setName(const std::string& name);

        auto getTag() const -> const std::string&;
        void setTag(const std::string& tag);

        template <typename T>
        bool has() const;

        template <typename T>
        auto get() const -> T*;

        template <typename T>
        auto add() const -> T*;

        template <typename T>
        void remove() const;

        operator u32() const;
        operator entt::entity() const;

    private:
        Scene* m_scene;
        entt::entity m_entity;
    };

    template <typename T>
    bool Entity::has() const
    {
        return m_scene->getRegistry().all_of<T>();
    }

    template <typename T>
    auto Entity::get() const -> T*
    {
        return m_scene->getRegistry().try_get<T>(m_entity);
    }

    template <typename T>
    auto Entity::add() const -> T*
    {
        m_scene->getRegistry().emplace<T>(m_entity);
        return m_scene->getRegistry().try_get<T>(m_entity);
    }

    template <typename T>
    void Entity::remove() const
    {
        m_scene->getRegistry().remove<T>(m_entity);
    }
}
