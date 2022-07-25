#include "pch.hpp"
#include "rune/scene/scene.hpp"

namespace Rune
{
    void Scene::init() {}

    void Scene::cleanup() {}

    void Scene::update() {}

    auto Scene::createEntity() -> Entity
    {
        Entity entity = { this, m_registry.create() };
        return entity;
    }

    void Scene::destroyEntity(const Entity& entity)
    {
        m_registry.destroy(entity);
    }
}
