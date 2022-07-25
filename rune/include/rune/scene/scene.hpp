﻿// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include <entt/entity/registry.hpp>

namespace Rune
{
    class Guid;
    class Entity;

    class Scene
    {
    public:
        Scene() = default;

        void init();
        void cleanup();
        void update();

        auto getRegistry() -> entt::registry&;

        auto createEntity(std::string name = "") -> Entity;
        void destroyEntity(const Entity& entity);

        auto getEntityByGuid(const Guid& guid) -> Entity;

    private:
        entt::registry m_registry;
    };
}