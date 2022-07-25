// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "rune/scene/scene.hpp"

#include "rune/core/time.hpp"
#include "rune/graphics/graphics.hpp"
#include "rune/scene/components.hpp"
#include "rune/scene/entity.hpp"
#include "rune/scripting/script_engine.hpp"

#include <spdlog/fmt/fmt.h>

namespace Rune
{
    void Scene::init() {}

    void Scene::cleanup() {}

    void Scene::update()
    {
        {
            auto view = m_registry.view<ScriptBehaviour>();
            for (const auto& entity : view)
            {
                Entity e = { this, entity };
                ScriptEngine::getInstance().onUpdateEntity(this, e, Time::getDeltaTime());
            }
        }

        {
            auto view = m_registry.view<Transform, MeshRenderer>();
            for (const auto& entity : view)
            {
                auto [transform, renderer] = view.get(entity);

                GraphicsSystem::getInstance().addRenderable(transform.getTransform(), renderer.mesh, renderer.material);
            }
        }
    }

    auto Scene::getRegistry() -> entt::registry&
    {
        return m_registry;
    }

    auto Scene::createEntity(std::string name) -> Entity
    {
        Entity entity = { this, m_registry.create() };

        // Should a default name be given to the entity
        if (name.empty())
        {
            entt::entity e = entity;
            name = fmt::format("Entity {}", static_cast<u64>(e));
        }

        // Add a header to the entity and assign a name
        auto* entityHeader = entity.add<EntityHeader>();
        entityHeader->guid = {};
        entityHeader->name = name;

        return entity;
    }

    void Scene::destroyEntity(const Entity& entity)
    {
        m_registry.destroy(entity);
    }

    auto Scene::getEntityByGuid(const Guid& guid) -> Entity
    {
        auto view = m_registry.view<EntityHeader>();
        for (const auto& entity : view)
        {
            auto [header] = view.get(entity);
            if (header.guid == guid)
                return Entity{ this, entity };
        }
        return {};
    }

}
