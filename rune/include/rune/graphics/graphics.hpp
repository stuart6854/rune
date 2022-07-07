#pragma once

#include "graphics.hpp"
#include "rune/defines.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "material.hpp"

#include <array>
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

    struct Light
    {
        glm::vec3 position;
        bool isDirectional;
        glm::vec3 direction;

        float constant;
        float linear;
        float quadratic;

        glm::vec3 diffuseColor;
        glm::vec3 specularColor;
    };

    struct Renderable
    {
        Mesh* mesh;
        MaterialInst* materialInst;

        glm::mat4 worldMatrix;
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

        void addRenderable(const glm::mat4& transform, Mesh* mesh, MaterialInst* material);

        void render();

    private:
        static void initRendererFactories();

        void onFramebufferSize(i32 width, i32 height) const;

        bool canFrustumCull() const;

        static auto buildInstanceKey() -> u32;

    private:
        RenderingApi m_renderingApi = RenderingApi::eNone;
        Owned<RendererBase> m_renderer;

        WindowSystem* m_window;

        struct InternalLight
        {
            glm::vec4 position;
            glm::vec4 direction;

            glm::vec4 diffuse;
            glm::vec4 specular;
        };

        struct Lighting
        {
            glm::vec3 ambient = { 1, 1, 1 };
            i32 lightCount = 0;
            std::array<InternalLight, 32> lights{};
        };

        struct Scene
        {
            glm::mat4 proj = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            Lighting lighting{};
        } m_scene{};

        struct DrawData
        {
            Mesh* mesh;
            MaterialInst* material;
            glm::mat4 transform;
        };

        struct DrawInstance
        {
            u32 key;
            size drawDataIndex;

            bool operator<(const DrawInstance& other) const;
        };

        std::vector<DrawData> m_drawData;

        std::vector<DrawInstance> m_shadowBucket;
        std::vector<DrawInstance> m_geometryBucket;
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
