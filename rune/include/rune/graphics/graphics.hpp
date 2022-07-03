#pragma once

#include "material.hpp"
#include "rune/defines.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
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

        void render(Mesh* mesh, MaterialInst* materialInst);

    private:
        static void initRendererFactories();

        void onFramebufferSize(i32 width, i32 height) const;

    private:
        RenderingApi m_renderingApi = RenderingApi::eNone;
        Owned<RendererBase> m_renderer;

        WindowSystem* m_window;
    };

    class RendererBase : public Mesh::Observer, public Texture::Observer, public Shader::Observer
    {
    public:
        ~RendererBase() override = default;

        virtual void init() = 0;
        virtual void cleanup() = 0;

        virtual void setWindow(WindowSystem* window) = 0;
        virtual void onFramebufferSize(i32 width, i32 height) = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        virtual void draw(Mesh* mesh, MaterialInst* materialInst) = 0;

        void destroying(const Mesh* mesh) override = 0;
        void changed(const Mesh* mesh) override = 0;

        void destroying(const Texture* texture) override = 0;
        void changed(const Texture* texture) override = 0;

        void destroying(const Shader* shader) override = 0;
        void changed(const Shader* shader) override = 0;
    };

}
