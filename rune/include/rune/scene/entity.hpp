// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/utility/guid.hpp"

#include <entt/entity/entity.hpp>

namespace Rune
{
    class Scene;

    class Entity
    {
    public:
        Entity();
        Entity(Scene* scene, entt::entity entity);
        Entity(const Entity&);

        auto getGuid() const -> const Guid&;

        operator u64() const;
        operator entt::entity() const;

    private:
        Scene* m_scene;
        entt::entity m_entity;

        Guid m_guid;
    };
}