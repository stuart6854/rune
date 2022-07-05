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
        m_projView = proj * view;
    }

    void GraphicsSystem::addRenderable(const Renderable& renderable)
    {
        // TODO: Check if can be Frustum culled?

        auto& internalRenderable = m_geometryBucket.emplace_back();
        internalRenderable.mesh = renderable.mesh;
        internalRenderable.materialInst = renderable.materialInst;
        internalRenderable.projView = m_projView;
        internalRenderable.model = renderable.modelMatrix;
    }

    void GraphicsSystem::render()
    {
        if (m_renderer == nullptr)
            return;

        m_renderer->beginFrame();

        for (const auto& renderable : m_geometryBucket)
        {
            renderable.materialInst->setMat4("u_renderer.viewProj", renderable.projView);
            renderable.materialInst->setMat4("u_object.model", renderable.model);

            m_renderer->draw(renderable.mesh, renderable.materialInst);
        }

        m_renderer->endFrame();

        m_geometryBucket.clear();
    }

    void GraphicsSystem::onFramebufferSize(const i32 width, const i32 height) const
    {
        if (m_renderer == nullptr)
            return;

        m_renderer->onFramebufferSize(width, height);
    }
}
