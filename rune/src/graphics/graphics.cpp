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

        m_sceneUbo = m_renderer->createBuffer(sizeof(Scene), &m_sceneData);
        m_lightingUbo = m_renderer->createBuffer(sizeof(Lighting), &m_lightingData);
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

    auto GraphicsSystem::getRenderer() const -> RendererBase*
    {
        return m_renderer.get();
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

    void GraphicsSystem::beginScene(const glm::mat4 proj, const glm::mat4& view, const Lighting& lighting)
    {
        m_sceneData.proj = proj;
        m_sceneData.view = view;
        // TODO: Upload to scene UBO

        m_lightingData = lighting;
        // TODO: Only upload required lights
        m_renderer->updateBuffer(m_lightingUbo, 0, sizeof(Lighting), &m_lightingData);
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

        m_drawData[0].material->setFloat3("u_material.diffuse", { 1, 1, 1 });
        m_drawData[0].material->setFloat("u_material.shininess", 32);

        for (const auto& instance : m_geometryBucket)
        {
            const auto& drawData = m_drawData[instance.drawDataIndex];
            m_renderer->bindMaterial(drawData.material);
            m_renderer->bindUniformBuffer(m_sceneUbo, 0);
            m_renderer->bindUniformBuffer(m_lightingUbo, 1);

            m_renderer->bindMesh(drawData.mesh);

            m_sceneData.world = drawData.transform;
            // TODO: Only upload what changed
            m_renderer->updateBuffer(m_sceneUbo, 0, sizeof(Scene), &m_sceneData);

            m_renderer->draw();
        }

        m_renderer->endFrame();

        m_lightingData.lightCount = 0;
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

        // CORE_LOG_TRACE("{:B}", key);

        return key;
    }

    bool GraphicsSystem::DrawInstance::operator<(const DrawInstance& other) const
    {
        return other.key < other.key;
    }
}
