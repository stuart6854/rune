#pragma once

#include "rune/graphics/graphics.hpp"
#include "rune/utility/storage.hpp"

#include <glad/glad.h>

namespace Rune
{
    class RendererOpenGl : public RendererBase
    {
    public:
        static auto create() -> Owned<RendererBase>;

        void init() override;
        void cleanup() override;

        void setWindow(WindowSystem* window) override;
        void onFramebufferSize(i32 width, i32 height) override;

        auto createBuffer(size initialSize, const void* initialData) -> u32 override;

        auto createTexture(i32 width, i32 height, TextureFormat format, const void* data) -> u32 override;
        void destroyTexture(i32 id) override;

        auto createMesh(MeshTopology topology,
                        const std::vector<Vertex>& vertices,
                        const std::vector<u16>& indices)
        -> u32 override;
        void destroyMesh(i32 id) override;

        void beginFrame() override;
        void endFrame() override;

        void draw() override;

    private:
        Storage<GLuint> m_textures;

        struct Mesh
        {
            GLenum topology;
            GLuint vao;
            GLuint ebo;
            GLuint vbo;

            GLsizei indexCount;
        };
        Storage<Mesh> m_meshes;
    };
}
