#pragma once

#include "entity.hpp"

#include <entt/entity/registry.hpp>

namespace Rune
{
    class Scene
    {
    public:
        Scene() = default;

        void init();
        void cleanup();
        void update();

        auto createEntity() -> Entity;
        void destroyEntity(const Entity& entity);

    private:
        entt::registry m_registry;
    };
}