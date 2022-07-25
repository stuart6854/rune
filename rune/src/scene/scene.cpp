#include "pch.hpp"
#include "rune/scene/scene.hpp"

#include "rune/graphics/graphics.hpp"
#include "rune/scene/components.hpp"
#include "rune/scene/entity.hpp"

#include <spdlog/fmt/fmt.h>

namespace Rune
{
    void Scene::init() {}

    void Scene::cleanup() {}

    void Scene::update()
    {
        {
            auto view = m_registry.view<Transform, MeshRenderer>();
            for (const auto& entity : view)
            {
                auto [transform, renderer] = view.get(entity);

                GraphicsSystem::getInstance().addRenderable(glm::mat4(1.0f), renderer.mesh, renderer.material);
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
        entityHeader->name = name;

        return entity;
    }

    void Scene::destroyEntity(const Entity& entity)
    {
        m_registry.destroy(entity);
    }
}
