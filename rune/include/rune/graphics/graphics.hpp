#pragma once

#include "rune/defines.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "material.hpp"

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

    struct Renderable
    {
        Mesh* mesh;
        MaterialInst* materialInst;

        glm::mat4 modelMatrix;
    };

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

        void beginScene(const glm::mat4 proj, const glm::mat4& view);

        void addRenderable(const Renderable& renderable);

        void render();

    private:
        static void initRendererFactories();

        void onFramebufferSize(i32 width, i32 height) const;

    private:
        RenderingApi m_renderingApi = RenderingApi::eNone;
        Owned<RendererBase> m_renderer;

        WindowSystem* m_window;

        glm::mat4 m_projView;

        struct InternalRenderable
        {
            Mesh* mesh;
            MaterialInst* materialInst;
            glm::mat4 mvp;
        };
        std::vector<InternalRenderable> m_shadowBucket;
        std::vector<InternalRenderable> m_geometryBucket;
    };

    class RendererBase : public Mesh::Observer, public Texture::Observer, public Shader::Observer, public MaterialInst::Observer
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

        void destroying(const MaterialInst* materialInst) override = 0;
        void uniformChanged(const MaterialInst* materialInst, u32 bufferIndex, u32 offset, u32 size) override = 0;
    };

}
