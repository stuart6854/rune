#pragma once

#include "rune/graphics/graphics.hpp"

#include <glad/glad.h>

namespace Rune
{
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

        void draw(Mesh* mesh, Texture* texture) override;

        void destroying(const Mesh* mesh) override;
        void changed(const Mesh* mesh) override;

        void destroying(const Texture* texture) override;
        void changed(const Texture* texture) override;

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
            GLuint id;
        };

        static auto createTexture(const Texture* texture) -> GlTexture;
        static void destroyTexture(const GlTexture& glTexture);

    private:
        struct RendererData
        {
            std::unordered_map<Guid, GlTexture> textures;
            std::unordered_map<Guid, GlMesh> meshes;
        };

        Owned<RendererData> m_data = nullptr;
    };
}
