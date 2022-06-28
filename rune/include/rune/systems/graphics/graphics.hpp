#pragma once

#include "rune/defines.hpp"
#include "texture.hpp"

#include <functional>

namespace Rune
{
    class WindowSystem;

    enum class RenderingApi : u8
    {
        eNone = 0,
        eOpenGL,
        eDirectX,
        eVulkan
    };

    class RendererBase;

    class GraphicsSystem
    {
    public:
        using RendererFactoryFunc = Owned<RendererBase> (*)();

    public:
        static auto getInstance() -> GraphicsSystem&;

        static bool registerRendererFactory(RenderingApi rendererName, const RendererFactoryFunc& factoryFunc);

        void init(RenderingApi renderingApi);
        void cleanup();

        auto getRenderingApi() const -> RenderingApi;
        void setRenderingApi(RenderingApi renderingApi);

        auto getWindow() const -> WindowSystem*;
        void setWindow(WindowSystem* window);

        /* Textures */
        auto createTexture(i32 width, i32 height, TextureFormat format, const void* data) const -> u32;
        void destroyTexture(i32 id) const;

        void render();

    private:
        static void initRendererFactories();

        void onFramebufferSize(i32 width, i32 height) const;

    private:
        RenderingApi m_renderingApi = RenderingApi::eNone;
        Owned<RendererBase> m_renderer;

        WindowSystem* m_window;
    };

    class RendererBase
    {
    public:
        virtual ~RendererBase() = default;

        virtual void init() = 0;
        virtual void cleanup() = 0;

        virtual void setWindow(WindowSystem* window) = 0;
        virtual void onFramebufferSize(i32 width, i32 height) = 0;

        virtual auto createBuffer(size initialSize, void* initialData) -> u32 = 0;

        virtual auto createTexture(i32 width, i32 height, TextureFormat format, const void* data) -> u32 = 0;
        virtual void destroyTexture(i32 id) = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        virtual void draw() = 0;
    };

}
