#pragma once

#include "rune/graphics/graphics.hpp"

#include <glad/glad.h>

namespace Rune
{
    class Material;
    class MaterialInst;

    class Renderer_OpenGL : public RendererBase
    {
    public:
        static auto create() -> Owned<RendererBase>;

        void init() override;
        void cleanup() override;

        void setWindow(WindowSystem* window) override;
        void onFramebufferSize(i32 width, i32 height) override;

        void beginFrame() override;
        void endFrame() override;

        void draw(Mesh* mesh, MaterialInst* materialInst) override;

        void destroying(const Mesh* mesh) override;
        void changed(const Mesh* mesh) override;

        void destroying(const Texture* texture) override;
        void changed(const Texture* texture) override;

        void destroying(const Shader* shader) override;
        void changed(const Shader* shader) override;

        void destroying(const MaterialInst* materialInst) override;
        void uniformChanged(const MaterialInst* materialInst, u32 bufferIndex, u32 offset, u32 size) override;

    private:
        struct GlMesh
        {
            GLenum topology;
            GLuint vao;
            GLuint ebo;
            GLuint vbo;

            GLsizei indexCount;
        };

        static auto createMesh(const Mesh* mesh) -> GlMesh;
        static void destroyMesh(const GlMesh& glMesh);

        struct GlTexture
        {
            GLuint texId;
        };

        bool hasTexture(const Texture* texture) const;
        auto getTexture(const Texture* texture) const -> const GlTexture*;
        auto createTexture(const Texture* texture) const -> const GlTexture&;
        static void destroyTexture(const GlTexture& glTexture);

        struct GlMaterial
        {
            GLuint program;

            bool depthTest;
            bool doubleSided;
        };
        struct GlMaterialInst
        {
            struct UniformBuffer
            {
                GLuint binding;
                GLuint bufferId;
            };
            std::vector<UniformBuffer> uniformBuffers;

            struct TextureSlot
            {
                std::string name;
                GLuint slot;
                GLuint texId;
            };
            std::vector<TextureSlot> textureSlots;
        };

        static auto createMaterial(const Material* material) -> GlMaterial;
        static auto destroyMaterial(const GlMaterial& glMaterial);

        auto createMaterialInstance(const MaterialInst* materialInst) const -> GlMaterialInst;
        auto destroyMaterialInstance(const GlMaterialInst& glMaterialInst);

    private:
        struct RendererData
        {
            std::unordered_map<Guid, GlTexture> textures;
            std::unordered_map<Guid, GlMaterial> materials;
            std::unordered_map<Guid, GlMaterialInst> materialInstances;
            std::unordered_map<Guid, GlMesh> meshes;
        };

        Owned<RendererData> m_data = nullptr;
    };
}
