#pragma once

#include "graphics.hpp"
#include "graphics.hpp"
#include "graphics.hpp"
#include "graphics.hpp"
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
        int isDirectional;
        glm::vec4 direction;

        float constant;
        float linear;
        float quadratic;

        glm::vec4 diffuseColor;
        glm::vec4 specularColor;
    };

    struct Lighting
    {
        glm::vec4 viewPos = {};
        glm::vec3 ambient = { 0.2, 0.2f, 0.2f };

        i32 lightCount = 0;
        std::array<Light, 32> lights{};
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

        auto getRenderer() const -> RendererBase*;

        auto getWindow() const -> WindowSystem*;
        void setWindow(WindowSystem* window);

        void beginScene(const glm::mat4 proj, const glm::mat4& view, const Lighting& lighting);

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

        struct Scene
        {
            glm::mat4 proj = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 world = glm::mat4(1.0f);
        };
        Scene m_sceneData{};
        u32 m_sceneUbo{};

        Lighting m_lightingData{};
        u32 m_lightingUbo{};

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

    class RendererBase
    {
    public:
        virtual ~RendererBase() = default;

        virtual void init() = 0;
        virtual void cleanup() = 0;

        virtual void setWindow(WindowSystem* window) = 0;
        virtual void onFramebufferSize(i32 width, i32 height) = 0;

        virtual auto createBuffer(size size, const void* data) -> u32 = 0;
        virtual void destroyBuffer(u32 id) = 0;
        virtual void updateBuffer(u32 id, size offset, size size, const void* data) = 0;

        virtual auto createMesh(const std::vector<Vertex>& vertices, const std::vector<u16>& indices, MeshTopology topology) -> u32 = 0;
        virtual void destroyMesh(u32 id) = 0;
        virtual void updateMeshVertices(u32 id, const std::vector<Vertex>& vertices) = 0;
        virtual void updateMeshIndices(u32 id, const std::vector<u16>& indices) = 0;

        virtual auto createMaterial(const std::vector<u8>& vertexCode, const std::vector<u8>& fragmentCode) -> u32 = 0;
        virtual void destroyMaterial(u32 id) = 0;

        virtual auto createTexture(u32 width, u32 height, TextureFormat format, const void* data) -> u32 = 0;
        virtual void destroyTexture(u32 id) = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        virtual void bindUniformBuffer(u32 id, u32 binding) = 0;
        virtual void bindMaterial(MaterialInst* material) = 0;
        virtual void bindMesh(Mesh* mesh) = 0;

        virtual void draw() = 0;
    };

}
