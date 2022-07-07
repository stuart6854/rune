#include "rune/graphics/graphics.hpp"

#include "rune/macros.hpp"
#include "rune/events/events.hpp"

namespace Rune
{
    static std::unordered_map<RenderingApi, GraphicsSystem::RendererFactoryFunc> s_factoryFuncsMap;

    auto GraphicsSystem::getInstance() -> GraphicsSystem&
    {
        static GraphicsSystem renderingSystem;
        return renderingSystem;
    }

    bool GraphicsSystem::registerRendererFactory(const RenderingApi rendererName, const RendererFactoryFunc& factoryFunc)
    {
        s_factoryFuncsMap[rendererName] = factoryFunc;
        return true;
    }

    void GraphicsSystem::init(const RenderingApi renderingApi)
    {
        initRendererFactories();
        setRenderingApi(renderingApi);

        EventSystem::listen<EventFramebufferSize>([this](const EventFramebufferSize& event)
                                                  { onFramebufferSize(event.width, event.height); });
    }

    void GraphicsSystem::cleanup()
    {
        m_renderingApi = RenderingApi::eNone;
        m_renderer = nullptr;
    }

    auto GraphicsSystem::getRenderingApi() const -> RenderingApi
    {
        return m_renderingApi;
    }

    void GraphicsSystem::setRenderingApi(const RenderingApi renderingApi)
    {
        // Check if trying to switch to active already active api
        if (renderingApi == m_renderingApi)
            return;

        if (m_renderer != nullptr)
        {
            // Shutdown active renderer
            m_renderer->cleanup();
            m_renderer = nullptr;
        }

        m_renderingApi = renderingApi;

        // Create and init renderer for new api
        auto factoryFunc = s_factoryFuncsMap[renderingApi];
        RUNE_ENG_ASSERT(factoryFunc != nullptr, "Renderer factory function is null!");

        m_renderer = factoryFunc();
        m_renderer->init();
        m_renderer->setWindow(m_window);
    }

    auto GraphicsSystem::getWindow() const -> WindowSystem*
    {
        return m_window;
    }

    void GraphicsSystem::setWindow(WindowSystem* window)
    {
        m_window = window;
        m_renderer->setWindow(m_window);
    }

    void GraphicsSystem::beginScene(const glm::mat4 proj, const glm::mat4& view)
    {
        m_scene.proj = proj;
        m_scene.view = view;
    }

    void GraphicsSystem::addRenderable(const glm::mat4& transform, Mesh* mesh, MaterialInst* material)
    {
        if (canFrustumCull())
        {
            // TODO: Maybe add to list of culled objects, for debugging
            return;
        }

        auto& drawData = m_drawData.emplace_back();
        drawData.transform = transform;
        drawData.mesh = mesh;
        drawData.material = material;

        DrawInstance instance{ .key = buildInstanceKey(), .drawDataIndex = m_drawData.size() - 1 };

        // Put instance in buckets
        m_shadowBucket.push_back(instance);
        m_geometryBucket.push_back(instance);
    }

    void GraphicsSystem::render()
    {
        if (m_renderer == nullptr)
            return;

        std::sort(m_shadowBucket.begin(), m_shadowBucket.end());
        std::sort(m_geometryBucket.begin(), m_geometryBucket.end());

        m_renderer->beginFrame();

        auto& lighting = m_scene.lighting;
        // TODO: Renderer dedicated uniform buffer, bound once (proj, view, lighting, etc.)

        // TODO: TEMPORARY
        lighting.ambient = { 0.1f, 0.1f, 0.1f };
        lighting.lightCount = 1;
        lighting.lights[0].position = { 5, 5, -3, 1 };
        lighting.lights[0].direction = glm::vec4{ -1, -1, 1, 1 };
        // lighting.lights[0].direction = {};
        lighting.lights[0].diffuse = { 1, 1, 1, 1 };
        lighting.lights[0].specular = { 1, 1, 1, 1 };

        glm::vec3 viewPos = glm::inverse(m_scene.view)[3];
        for (const auto& instance : m_geometryBucket)
        {
            const auto& drawData = m_drawData[instance.drawDataIndex];
            drawData.material->setMat4("u_renderer.worldMatrix", drawData.transform);
            drawData.material->setMat4("u_renderer.viewMatrix", m_scene.view);
            drawData.material->setMat4("u_renderer.projMatrix", m_scene.proj);
            drawData.material->setFloat3("u_lighting.viewPos", viewPos);
            drawData.material->setFloat3("u_lighting.ambient", lighting.ambient);
            drawData.material->setInt("u_lighting.lightCount", lighting.lightCount);
            drawData.material->setData("u_lighting.lights", sizeof(InternalLight) * lighting.lightCount, lighting.lights.data());

            drawData.material->setFloat3("u_material.diffuse", { 1, 1, 1 });
            drawData.material->setFloat("u_material.shininess", 32);

            m_renderer->draw(drawData.mesh, drawData.material);
        }

        m_renderer->endFrame();

        lighting.lightCount = 0;
        m_shadowBucket.clear();
        m_geometryBucket.clear();
        m_drawData.clear();
    }

    void GraphicsSystem::onFramebufferSize(const i32 width, const i32 height) const
    {
        if (m_renderer == nullptr)
            return;

        m_renderer->onFramebufferSize(width, height);
    }

    bool GraphicsSystem::canFrustumCull() const
    {
        // TODO: Check if can be Frustum culled?
        // TODO: Meshes need a bounding box (local space) (AABB?)
        return false;
    }

    auto GraphicsSystem::buildInstanceKey() -> u32
    {
        u32 key = 0;

        bool isTransparent = true;
        u8 materialIndex = 255;
        u8 meshIndex = 4;

        key = (key | isTransparent) << 8;
        key = (key | materialIndex) << 8;
        key = (key | meshIndex);

        CORE_LOG_TRACE("{:B}", key);

        return key;
    }

    bool GraphicsSystem::DrawInstance::operator<(const DrawInstance& other) const
    {
        return other.key < other.key;
    }
}
