#include "rune/systems/graphics/graphics.hpp"

#include "rune/macros.hpp"

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

    void GraphicsSystem::render()
    {
        if (m_renderer == nullptr)
            return;

        m_renderer->beginFrame();
        m_renderer->draw();
        m_renderer->endFrame();
    }
}
