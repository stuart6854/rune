#include "renderer.hpp"

#include "rune/macros.hpp"
#include "rune/graphics/material.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

namespace Rune
{
    namespace
    {
        void checkForShaderError(const u32 shader)
        {
            int success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                CORE_LOG_ERROR("Shader Compile Error: \n{}", infoLog);
            }
        }

        void checkForProgramError(const u32 program)
        {
            int success;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetProgramInfoLog(program, 512, nullptr, infoLog);
                CORE_LOG_ERROR("Program Link Error: \n{}", infoLog);
            }
        }

#ifdef _DEBUG
        void openglMessageCallback(
            GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* msg, void const* userParam)
        {
            // ignore non-significant error/warning codes
            if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
                return;

            std::string severityStr;
            switch (severity)
            {
                case GL_DEBUG_SEVERITY_HIGH: severityStr = "high"; break;
                case GL_DEBUG_SEVERITY_MEDIUM: severityStr = "medium"; break;
                case GL_DEBUG_SEVERITY_LOW: severityStr = "low"; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "notification"; break;
                default: severityStr = "Unknown"; break;
            }

            std::string sourceStr;
            switch (src)
            {
                case GL_DEBUG_SOURCE_API: sourceStr = "API"; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "Window System"; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "Third Party"; break;
                case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "Application"; break;
                case GL_DEBUG_SOURCE_OTHER: sourceStr = "Other"; break;
                default: sourceStr = "Unknown"; break;
            }

            std::string typeStr;
            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR: typeStr = "Error"; break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behaviour"; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = " Undefined Behaviour"; break;
                case GL_DEBUG_TYPE_PORTABILITY: typeStr = "Portability"; break;
                case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "Performance"; break;
                case GL_DEBUG_TYPE_MARKER: typeStr = "Marker"; break;
                case GL_DEBUG_TYPE_PUSH_GROUP: typeStr = "Push Group"; break;
                case GL_DEBUG_TYPE_POP_GROUP: typeStr = "Pop Group"; break;
                case GL_DEBUG_TYPE_OTHER: typeStr = "Other"; break;
                default: typeStr = "Unknown"; break;
            }

            CORE_LOG_ERROR("--------------------");
            CORE_LOG_ERROR("OpenGL Debug Message ({}) : {}", id, msg);
            CORE_LOG_ERROR("Source: {}", sourceStr);
            CORE_LOG_ERROR("Type: {}", typeStr);
            CORE_LOG_ERROR("Severity: {}", severityStr);
            CORE_LOG_ERROR("--------------------");
        }
#endif

        auto toGLInternalTextureFormat(const TextureFormat format) -> GLint
        {
            switch (format)
            {
                case TextureFormat::eR: return GL_R8;
                case TextureFormat::eRGB: return GL_RGB8;
                case TextureFormat::eRGBA: return GL_RGBA8;
                case TextureFormat::eUnknown: return 0;
            }
            return 0;
        }

        auto toGLTextureFormat(const TextureFormat format) -> GLint
        {
            switch (format)
            {
                case TextureFormat::eR: return GL_RED;
                case TextureFormat::eRGB: return GL_RGB;
                case TextureFormat::eRGBA: return GL_RGBA;
                case TextureFormat::eUnknown: return 0;
            }
            return 0;
        }

        auto toGLTopology(const MeshTopology topology) -> GLenum
        {
            switch (topology)
            {
                case MeshTopology::eLines: return GL_LINES;
                case MeshTopology::eTriangles: return GL_TRIANGLES;
                case MeshTopology::eNone: break;
            }
            return GL_NONE;
        }
    }

    auto Renderer_OpenGL::create() -> Owned<RendererBase>
    {
        return CreateOwned<Renderer_OpenGL>();
    }

    void Renderer_OpenGL::init()
    {
        m_data = CreateOwned<RendererData>();

        bool isOpenGlLoaded = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        RUNE_ENG_ASSERT(isOpenGlLoaded, "Failed to load OpenGL!");

        CORE_LOG_INFO("OpenGL Version {}.{}", GLVersion.major, GLVersion.minor);

#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(openglMessageCallback, nullptr);
#endif

        glEnable(GL_DEPTH_TEST);
    }

    void Renderer_OpenGL::cleanup()
    {
        m_data.reset();
    }

    void Renderer_OpenGL::setWindow(WindowSystem* window) {}

    void Renderer_OpenGL::onFramebufferSize(const i32 width, const i32 height)
    {
        glViewport(0, 0, width, height);
    }

    void Renderer_OpenGL::beginFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3912f, 0.5843f, 0.9294f, 1.0f);  // Cornflower Blue
    }

    void Renderer_OpenGL::endFrame() {}

    void Renderer_OpenGL::draw(Mesh* mesh, MaterialInst* materialInst)
    {
        /* Mesh */
        if (!m_data->meshes.contains(mesh->getGuid()))
        {
            // Create OpenGL mesh
            m_data->meshes.emplace(mesh->getGuid(), createMesh(mesh));

            mesh->attachObserver(this);
        }

        /* Material Instance */
        if (!m_data->materialInstances.contains(materialInst->getGuid()))
        {
            // Create OpenGL material instance
            m_data->materialInstances.emplace(materialInst->getGuid(), createMaterialInstance(materialInst));

            materialInst->attachObserver(this);
        }

        /* Texture */
        /*if (!m_data->textures.contains(texture->getGuid()))
        {
            // Create OpenGL texture
            m_data->textures.emplace(texture->getGuid(), createTexture(texture));

            materialInst->attachObserver(this);
        }*/

        auto& glMaterial = m_data->materials.at(materialInst->getMaterial()->getGuid());
        glUseProgram(glMaterial.program);

        auto& glMaterialInst = m_data->materialInstances.at(materialInst->getGuid());

        // Uniform Buffers
        for (const auto& uniformBuffer : glMaterialInst.uniformBuffers)
        {
            glUniformBlockBinding(glMaterial.program, 0, uniformBuffer.binding);
            glBindBufferBase(GL_UNIFORM_BUFFER, uniformBuffer.binding, uniformBuffer.bufferId);
        }

        // Textures
        for (const auto& slot : glMaterialInst.textureSlots)
        {
            glBindTextureUnit(slot.slot, slot.texId);
            glUniform1i(glGetUniformLocation(glMaterial.program, slot.name.c_str()), slot.slot);
        }

        auto& glMesh = m_data->meshes.at(mesh->getGuid());

        glBindVertexArray(glMesh.vao);
        glDrawElements(glMesh.topology, glMesh.indexCount, GL_UNSIGNED_SHORT, nullptr);
    }

    void Renderer_OpenGL::destroying(const Mesh* mesh)
    {
        const auto it = m_data->meshes.find(mesh->getGuid());
        if (it != m_data->meshes.end())
        {
            destroyMesh(it->second);
            m_data->meshes.erase(it);
        }
    }

    void Renderer_OpenGL::changed(const Mesh* mesh)
    {
        const auto& glMesh = m_data->meshes.at(mesh->getGuid());
        destroyMesh(glMesh);
        m_data->meshes.emplace(mesh->getGuid(), createMesh(mesh));
    }

    void Renderer_OpenGL::destroying(const Texture* texture) {}

    void Renderer_OpenGL::changed(const Texture* texture) {}

    void Renderer_OpenGL::destroying(const Shader* shader) {}

    void Renderer_OpenGL::changed(const Shader* shader) {}

    void Renderer_OpenGL::destroying(const MaterialInst* materialInst) {}

    void Renderer_OpenGL::uniformChanged(const MaterialInst* materialInst, const u32 bufferIndex, const u32 offset, const u32 size)
    {
        const auto& buffer = materialInst->getUniformBuffers()[bufferIndex].buffer;

        auto& glMaterialInst = m_data->materialInstances.at(materialInst->getGuid());
        glNamedBufferSubData(glMaterialInst.uniformBuffers[bufferIndex].bufferId, offset, size, buffer.getData());
    }

    auto Renderer_OpenGL::createMesh(const Mesh* mesh) -> GlMesh
    {
        const auto& indices = mesh->getIndices();
        const auto& vertices = mesh->getVertices();

        GlMesh glMesh{};

        glMesh.topology = toGLTopology(mesh->getTopology());
        glMesh.indexCount = indices.size();

        glCreateVertexArrays(1, &glMesh.vao);

        // Index Buffer
        glCreateBuffers(1, &glMesh.ebo);
        glNamedBufferData(glMesh.ebo, sizeof(u16) * indices.size(), indices.data(), GL_STATIC_DRAW);
        // Vertex buffer
        glCreateBuffers(1, &glMesh.vbo);
        glNamedBufferData(glMesh.vbo, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        // Assign buffers to Vertex Array
        glVertexArrayVertexBuffer(glMesh.vao, 0, glMesh.vbo, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(glMesh.vao, glMesh.ebo);

        // Setup attributes
        glEnableVertexArrayAttrib(glMesh.vao, 0);
        glVertexArrayAttribBinding(glMesh.vao, 0, 0);
        glVertexArrayAttribFormat(glMesh.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));

        glEnableVertexArrayAttrib(glMesh.vao, 1);
        glVertexArrayAttribBinding(glMesh.vao, 1, 0);
        glVertexArrayAttribFormat(glMesh.vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

        glEnableVertexArrayAttrib(glMesh.vao, 2);
        glVertexArrayAttribBinding(glMesh.vao, 2, 0);
        glVertexArrayAttribFormat(glMesh.vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, norm));

        return glMesh;
    }

    void Renderer_OpenGL::destroyMesh(const Renderer_OpenGL::GlMesh& glMesh)
    {
        glDeleteBuffers(1, &glMesh.ebo);
        glDeleteBuffers(1, &glMesh.vbo);
        glDeleteVertexArrays(1, &glMesh.vao);
    }

    bool Renderer_OpenGL::hasTexture(const Texture* texture) const
    {
        return m_data->textures.contains(texture->getGuid());
    }

    auto Renderer_OpenGL::getTexture(const Texture* texture) const -> const Renderer_OpenGL::GlTexture*
    {
        if (hasTexture(texture))
            return &m_data->textures.at(texture->getGuid());

        return nullptr;
    }

    auto Renderer_OpenGL::createTexture(const Texture* texture) const -> const GlTexture&
    {
        GLuint textureId;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureId);

        glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        auto width = texture->getWidth();
        auto height = texture->getHeight();
        auto internalFormat = toGLInternalTextureFormat(texture->getFormat());
        auto format = toGLTextureFormat(texture->getFormat());

        glTextureStorage2D(textureId, 1, internalFormat, width, height);
        glTextureSubImage2D(textureId, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texture->getData().data());

        glGenerateTextureMipmap(textureId);

        GlTexture glTexture{ textureId };

        m_data->textures[texture->getGuid()] = glTexture;
        return m_data->textures[texture->getGuid()];
    }

    void Renderer_OpenGL::destroyTexture(const Renderer_OpenGL::GlTexture& glTexture)
    {
        glDeleteTextures(1, &glTexture.texId);
    }

    auto Renderer_OpenGL::createMaterial(const Material* material) -> GlMaterial
    {
        GlMaterial glMaterial{};

        auto* shader = material->getShader();
        const auto& vertexCode = shader->getVertexCode();
        const auto& fragmentCode = shader->getFragmentCode();

        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderBinary(1, &vertShader, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexCode.data(), vertexCode.size() * sizeof(u8));
        glSpecializeShader(vertShader, "main", 0, nullptr, nullptr);
        checkForShaderError(vertShader);

        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderBinary(1, &fragShader, GL_SHADER_BINARY_FORMAT_SPIR_V, fragmentCode.data(), fragmentCode.size() * sizeof(u8));
        glSpecializeShader(fragShader, "main", 0, nullptr, nullptr);
        checkForShaderError(fragShader);

        glMaterial.program = glCreateProgram();
        glAttachShader(glMaterial.program, vertShader);
        glAttachShader(glMaterial.program, fragShader);
        glLinkProgram(glMaterial.program);
        checkForProgramError(glMaterial.program);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return glMaterial;
    }

    auto Renderer_OpenGL::destroyMaterial(const GlMaterial& glMaterial) {}

    auto Renderer_OpenGL::createMaterialInstance(const MaterialInst* materialInst) const -> GlMaterialInst
    {
        // Create the base material if it doesn't exist
        auto* material = materialInst->getMaterial();
        if (!m_data->materials.contains(material->getGuid()))
        {
            // Create OpenGL material
            m_data->materials.emplace(material->getGuid(), createMaterial(material));

            // material->attachObserver(this); // TODO
        }

        /* Create material instance */

        const auto& baseUniformBuffers = material->getUniformBuffers();
        const auto& baseTextureSlots = material->getTextureSlots();

        const auto& instUniformBuffers = materialInst->getUniformBuffers();
        const auto& instTextureSlots = materialInst->getTextureSlots();

        GlMaterialInst glMaterialInst{};

        /* Uniform Buffers */

        glMaterialInst.uniformBuffers.resize(baseUniformBuffers.size());
        for (size i = 0; i < baseUniformBuffers.size(); ++i)
        {
            // Set from Material Inst
            const auto& uniformBuffer = instUniformBuffers[i];

            glMaterialInst.uniformBuffers[i].binding = uniformBuffer.binding;
            // Create opengl buffer
            glCreateBuffers(1, &glMaterialInst.uniformBuffers[i].bufferId);
            glNamedBufferData(
                glMaterialInst.uniformBuffers[i].bufferId, uniformBuffer.buffer.getSize(), uniformBuffer.buffer.getData(), GL_STATIC_DRAW);
        }

        /* Texture Slots */
        glMaterialInst.textureSlots.resize(baseTextureSlots.size());
        for (size i = 0; i < baseTextureSlots.size(); ++i)
        {
            const auto& baseSlot = baseTextureSlots[i];

            /* Set from Material Base */

            if (baseSlot.texture != nullptr)
            {
                // Create texture if it does not exist in OpenGL
                GLuint textureId;
                if (!hasTexture(baseSlot.texture))
                    textureId = createTexture(baseSlot.texture).texId;
                else
                    textureId = getTexture(baseSlot.texture)->texId;

                glMaterialInst.textureSlots[i] = { baseSlot.name, baseSlot.binding, textureId };
            }

            const auto& instSlot = instTextureSlots[i];

            if (instSlot.texture != nullptr)
            {
                /* Overwrite with Material Inst (if different) */
                if (instSlot.texture != baseSlot.texture)
                {
                    // Create texture if it does not exist in OpenGL
                    GLuint textureId;
                    if (!hasTexture(instSlot.texture))
                        textureId = createTexture(instSlot.texture).texId;
                    else
                        textureId = getTexture(instSlot.texture)->texId;

                    glMaterialInst.textureSlots[i] = { instSlot.name, instSlot.binding, textureId };
                }
            }
        }

        return glMaterialInst;
    }

    auto Renderer_OpenGL::destroyMaterialInstance(const GlMaterialInst& glMaterialInst) {}

}
