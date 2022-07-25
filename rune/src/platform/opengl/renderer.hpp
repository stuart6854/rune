// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/graphics/graphics.hpp"
#include "rune/utility/storage.hpp"

#include <glad/glad.h>

namespace Rune
{
    class Material;
    class MaterialInst;

    class Renderer_OpenGL : public RendererBase
    {
    public:
        static auto create() -> Owned<RendererBase>;

        ~Renderer_OpenGL() override;

        void init() override;
        void cleanup() override;

        void setWindow(WindowSystem* window) override;
        void onFramebufferSize(i32 width, i32 height) override;

        auto createBuffer(size size, const void* data) -> u32 override;
        void destroyBuffer(u32 id) override;
        void updateBuffer(u32 id, size offset, size size, const void* data) override;

        auto createMesh(const std::vector<Vertex>& vertices, const std::vector<u16>& indices, MeshTopology topology) -> u32 override;
        void destroyMesh(u32 id) override;
        void updateMeshVertices(u32 id, const std::vector<Vertex>& vertices) override;
        void updateMeshIndices(u32 id, const std::vector<u16>& indices) override;

        auto createMaterial(const std::vector<u8>& vertexCode, const std::vector<u8>& fragmentCode) -> u32 override;
        void destroyMaterial(u32 id) override;

        auto createTexture(u32 width, u32 height, TextureFormat format, const void* data) -> u32 override;
        void destroyTexture(u32 id) override;

        void beginFrame() override;
        void endFrame() override;

        void bindUniformBuffer(u32 id, u32 binding) override;
        void bindMaterial(MaterialInst* material) override;
        void bindMesh(Mesh* mesh) override;

        void draw() override;

    private:
        struct Buffer
        {
            GLuint buffer;
            GLenum type;
            GLsizei size;
        };

        struct Mesh
        {
            GLenum topology;
            GLuint vao;
            u32 indexBuffer;
            u32 vertexBuffer;
            GLsizei indexCount;
        };

        struct Material
        {
            GLuint program;

            bool depthTest;
            bool doubleSided;
        };

        struct Texture
        {
            GLuint texture;
        };

    private:
        Storage<Buffer> m_bufferStorage;
        Storage<Mesh> m_meshStorage;
        Storage<Material> m_materialStorage;
        Storage<Texture> m_textureStorage;

        Material* m_boundMaterial = nullptr;
        Mesh* m_boundMesh = nullptr;
    };
}
