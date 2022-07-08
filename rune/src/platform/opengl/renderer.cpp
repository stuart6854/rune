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

    Renderer_OpenGL::~Renderer_OpenGL() {}

    void Renderer_OpenGL::init()
    {
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
        // TODO: Destroy resources
    }

    void Renderer_OpenGL::setWindow(WindowSystem* window) {}

    void Renderer_OpenGL::onFramebufferSize(const i32 width, const i32 height)
    {
        glViewport(0, 0, width, height);
    }

    u32 Renderer_OpenGL::createBuffer(const size size, const void* data)
    {
        Buffer buffer{};
        buffer.size = size;

        glCreateBuffers(1, &buffer.buffer);
        glNamedBufferData(buffer.buffer, size, data, GL_STATIC_DRAW);

        return m_bufferStorage.add(buffer);
    }

    void Renderer_OpenGL::destroyBuffer(const u32 id)
    {
        auto& buffer = m_bufferStorage.get(id);
        glDeleteBuffers(1, &buffer.buffer);

        m_bufferStorage.remove(id);
    }

    void Renderer_OpenGL::updateBuffer(const u32 id, size offset, size size, const void* data)
    {
        auto& buffer = m_bufferStorage.get(id);

        glNamedBufferData(buffer.buffer, size, data, GL_STATIC_DRAW);
    }

    auto Renderer_OpenGL::createMesh(const std::vector<Vertex>& vertices, const std::vector<u16>& indices, const MeshTopology topology)
        -> u32
    {
        Mesh mesh{};

        mesh.topology = toGLTopology(topology);
        mesh.indexCount = indices.size();

        glCreateVertexArrays(1, &mesh.vao);

        mesh.indexBuffer = createBuffer(indices.size() * sizeof(u16), indices.data());
        mesh.vertexBuffer = createBuffer(vertices.size() * sizeof(Vertex), vertices.data());

        auto& ibo = m_bufferStorage.get(mesh.indexBuffer);
        auto& vbo = m_bufferStorage.get(mesh.vertexBuffer);

        // Assign buffers to Vertex Buffer Object
        glVertexArrayElementBuffer(mesh.vao, ibo.buffer);
        glVertexArrayVertexBuffer(mesh.vao, 0, vbo.buffer, 0, sizeof(Vertex));

        // Setup attributes
        glEnableVertexArrayAttrib(mesh.vao, 0);
        glVertexArrayAttribBinding(mesh.vao, 0, 0);
        glVertexArrayAttribFormat(mesh.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));

        glEnableVertexArrayAttrib(mesh.vao, 1);
        glVertexArrayAttribBinding(mesh.vao, 1, 0);
        glVertexArrayAttribFormat(mesh.vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

        glEnableVertexArrayAttrib(mesh.vao, 2);
        glVertexArrayAttribBinding(mesh.vao, 2, 0);
        glVertexArrayAttribFormat(mesh.vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, norm));

        return m_meshStorage.add(mesh);
    }

    void Renderer_OpenGL::destroyMesh(const u32 id)
    {
        auto& mesh = m_meshStorage.get(id);

        destroyBuffer(mesh.vertexBuffer);
        destroyBuffer(mesh.indexBuffer);

        glDeleteVertexArrays(1, &mesh.vao);
        m_meshStorage.remove(id);
    }

    void Renderer_OpenGL::updateMeshVertices(const u32 id, const std::vector<Vertex>& vertices)
    {
        auto& mesh = m_meshStorage.get(id);
        updateBuffer(mesh.vertexBuffer, 0, sizeof(Vertex) * vertices.size(), vertices.data());
    }

    void Renderer_OpenGL::updateMeshIndices(const u32 id, const std::vector<u16>& indices)
    {
        auto& mesh = m_meshStorage.get(id);
        updateBuffer(mesh.indexBuffer, 0, sizeof(u16) * indices.size(), indices.data());
    }

    auto Renderer_OpenGL::createMaterial(const std::vector<u8>& vertexCode, const std::vector<u8>& fragmentCode) -> u32
    {
        Material material{};

        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderBinary(1, &vertShader, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexCode.data(), vertexCode.size() * sizeof(u8));
        glSpecializeShader(vertShader, "main", 0, nullptr, nullptr);
        checkForShaderError(vertShader);

        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderBinary(1, &fragShader, GL_SHADER_BINARY_FORMAT_SPIR_V, fragmentCode.data(), fragmentCode.size() * sizeof(u8));
        glSpecializeShader(fragShader, "main", 0, nullptr, nullptr);
        checkForShaderError(fragShader);

        material.program = glCreateProgram();
        glAttachShader(material.program, vertShader);
        glAttachShader(material.program, fragShader);
        glLinkProgram(material.program);
        checkForProgramError(material.program);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return m_materialStorage.add(material);
    }

    void Renderer_OpenGL::destroyMaterial(const u32 id)
    {
        auto& material = m_materialStorage.get(id);

        glDeleteProgram(material.program);

        m_materialStorage.remove(id);
    }

    auto Renderer_OpenGL::createTexture(const u32 width, const u32 height, const TextureFormat format, const void* data) -> u32
    {
        Texture texture{};

        glCreateTextures(GL_TEXTURE_2D, 1, &texture.texture);

        glTextureParameteri(texture.texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(texture.texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(texture.texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(texture.texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

        auto internalFormat = toGLInternalTextureFormat(format);
        auto dataFormat = toGLTextureFormat(format);

        glTextureStorage2D(texture.texture, 1, internalFormat, width, height);
        glTextureSubImage2D(texture.texture, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

        glGenerateTextureMipmap(texture.texture);

        return m_textureStorage.add(texture);
    }

    void Renderer_OpenGL::destroyTexture(const u32 id)
    {
        auto& texture = m_textureStorage.get(id);
        glDeleteTextures(1, &texture.texture);

        m_textureStorage.remove(id);
    }

    void Renderer_OpenGL::beginFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3912f, 0.5843f, 0.9294f, 1.0f);  // Cornflower Blue
    }

    void Renderer_OpenGL::endFrame() {}

    void Renderer_OpenGL::bindUniformBuffer(const u32 id, const u32 binding)
    {
        auto& buffer = m_bufferStorage.get(id);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer.buffer);
    }

    void Renderer_OpenGL::bindMaterial(MaterialInst* material)
    {
        auto& internalMaterial = m_materialStorage.get(material->getMaterial()->getId());
        if (m_boundMaterial == &internalMaterial)
            return;

        glUseProgram(internalMaterial.program);

        // TODO: Set state

        // Uniform Buffers
        for (const auto& uniformBuffer : material->getUniformBuffers())
        {
            bindUniformBuffer(uniformBuffer.internalId, uniformBuffer.binding);
        }

        // Textures
        for (const auto& slot : material->getTextureSlots())
        {
            auto& texture = m_textureStorage.get(slot.texture->getInternalId());
            glBindTextureUnit(slot.binding, texture.texture);
            glUniform1i(glGetUniformLocation(internalMaterial.program, slot.name.c_str()), slot.binding);
        }

        m_boundMaterial = &internalMaterial;
    }

    void Renderer_OpenGL::bindMesh(Rune::Mesh* mesh)
    {
        auto& internalMesh = m_meshStorage.get(mesh->getId());
        if (m_boundMesh == &internalMesh)
            return;

        glBindVertexArray(internalMesh.vao);

        m_boundMesh = &internalMesh;
    }

    void Renderer_OpenGL::draw()
    {
        RUNE_ENG_ASSERT(m_boundMaterial != nullptr, "No material bound!");
        RUNE_ENG_ASSERT(m_boundMesh != nullptr, "No mesh bound!");

        glDrawElements(m_boundMesh->topology, m_boundMesh->indexCount, GL_UNSIGNED_SHORT, nullptr);
    }

    /*void Renderer_OpenGL::uniformChanged(const MaterialInst* materialInst, const u32 bufferIndex, const u32 offset, const u32 size)
    {
        const auto& buffer = materialInst->getUniformBuffers()[bufferIndex].buffer;

        auto& glMaterialInst = m_data->materialInstances.at(materialInst->getGuid());
        glNamedBufferSubData(glMaterialInst.uniformBuffers[bufferIndex].bufferId, offset, size, buffer.readBytes(size, offset));
    }*/

    /*auto Renderer_OpenGL::createMaterialInstance(const MaterialInst* materialInst) const -> GlMaterialInst
    {
        // Create the base material if it doesn't exist
        auto* material = materialInst->getMaterial();
        if (!m_data->materials.contains(material->getGuid()))
        {
            // Create OpenGL material
            m_data->materials.emplace(material->getGuid(), createMaterial(material));

            // material->attachObserver(this); // TODO
        }

        /* Create material instance #1#

        const auto& baseUniformBuffers = material->getUniformBuffers();
        const auto& baseTextureSlots = material->getTextureSlots();

        const auto& instUniformBuffers = materialInst->getUniformBuffers();
        const auto& instTextureSlots = materialInst->getTextureSlots();

        GlMaterialInst glMaterialInst{};

        /* Uniform Buffers #1#

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

        /* Texture Slots #1#
        glMaterialInst.textureSlots.resize(baseTextureSlots.size());
        for (size i = 0; i < baseTextureSlots.size(); ++i)
        {
            const auto& baseSlot = baseTextureSlots[i];

            /* Set from Material Base #1#

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
                /* Overwrite with Material Inst (if different) #1#
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
    }*/

}
