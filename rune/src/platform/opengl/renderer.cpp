#include "renderer.hpp"
#include "renderer.hpp"

#include "GLFW/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "rune/macros.hpp"

#include <glad/glad.h>
#include <spirv_reflect.h>

#include <unordered_map>

const std::string vert_shader_src = R"(
#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_norm;

layout(location = 0) out vec2 out_uv;

uniform mat4 u_mvp;

void main()
{
	out_uv = a_uv;
	gl_Position = u_mvp * vec4(a_pos, 1.0);
}
)";

const std::string frag_shader_src = R"(
#version 450 core

layout(location = 0) in vec2 in_uv;

uniform sampler2D tex;

out vec4 out_fragColor;

void main()
{
	vec4 color = texture(tex, in_uv);
	out_fragColor = color; //vec4(1.0, 0.5, 0.2, 1.0);
}
)";

GLuint shaderProgram;

Rune::Shader shader;
inline std::vector<u8> readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        CORE_LOG_ERROR("Failed to open shader file: {}", filename);
        return {};
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<u8> buffer(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    file.close();

    return buffer;
};

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

        auto toUniformType(SpvReflectDescriptorType spirvType) -> BindingType
        {
            switch (spirvType)
            {
                case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER: break;
                case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return BindingType::eTexture;
                case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE: break;
                case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE: break;
                case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: break;
                case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: break;
                case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return BindingType::eUniformBuffer;
                case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER: break;
                case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: break;
                case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: break;
                case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: break;
                case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR: break;
                default: break;
            }
            return BindingType::eNone;
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

        {
            // TODO: TEST CODE TO BE REMOVED

            auto vertSrc = readFile("assets/default.vert.spv");
            auto fragSrc = readFile("assets/default.frag.spv");

            u32 vertShader = glCreateShader(GL_VERTEX_SHADER);
            // glShaderSource(vertShader, 1, vert_shader_src.data(), nullptr);
            glShaderBinary(1, &vertShader, GL_SHADER_BINARY_FORMAT_SPIR_V, vertSrc.data(), vertSrc.size() * sizeof(u8));
            // glCompileShader(vertShader);
            glSpecializeShader(vertShader, "main", 0, nullptr, nullptr);
            checkForShaderError(vertShader);

            u32 fragShader = glCreateShader(GL_FRAGMENT_SHADER);
            // glShaderSource(fragShader, 1, &frag_shader_src, nullptr);
            glShaderBinary(1, &fragShader, GL_SHADER_BINARY_FORMAT_SPIR_V, fragSrc.data(), fragSrc.size() * sizeof(u8));
            // glCompileShader(fragShader);
            glSpecializeShader(fragShader, "main", 0, nullptr, nullptr);
            checkForShaderError(fragShader);

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertShader);
            glAttachShader(shaderProgram, fragShader);
            glLinkProgram(shaderProgram);
            checkForProgramError(shaderProgram);

            glDeleteShader(vertShader);
            glDeleteShader(fragShader);

            shader.init(vertSrc);
            shader.setReflectionData(reflect(&shader));
        }
    }

    void Renderer_OpenGL::cleanup()
    {
        m_data.reset();
    }

    void Renderer_OpenGL::setWindow(WindowSystem* window) {}

    void Renderer_OpenGL::onFramebufferSize(i32 width, i32 height) {}

    void Renderer_OpenGL::beginFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.3912f, 0.5843f, 0.9294f, 1.0f);  // Cornflower Blue
    }

    void Renderer_OpenGL::endFrame() {}

    void Renderer_OpenGL::draw(Mesh* mesh, Texture* texture)
    {
        if (!m_data->meshes.contains(mesh->getGuid()))
        {
            // Create OpenGL mesh
            m_data->meshes.emplace(mesh->getGuid(), createMesh(mesh));

            mesh->attachObserver(this);
        }

        if (!m_data->textures.contains(texture->getGuid()))
        {
            // Create OpenGL texture
            m_data->textures.emplace(texture->getGuid(), createTexture(texture));

            texture->attachObserver(this);
        }

        glUseProgram(shaderProgram);

        // Texture
        glBindTextureUnit(0, 1);
        auto x = glGetUniformLocation(shaderProgram, "tex");
        glUniform1i(x, 0);

        // MVP
        auto proj = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
        auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        auto mvp = proj * view;
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "u_uniforms.mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

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

    auto Renderer_OpenGL::reflect(const Shader* shader) -> ReflectionData
    {
        ReflectionData reflectionData;

        /*GLuint programId = shaderProgram;

        GLint uniformCount = 0;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);

        if (uniformCount != 0)
        {
            GLint maxNameLen = 0;  // The length of the longest uniform name
            GLsizei length = 0;    // Length of the current uniforms name
            GLsizei count = 0;
            GLenum type = GL_NONE;
            glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLen);

            std::string nameBuffer;
            nameBuffer.resize(maxNameLen);

            reflectionData.buffers.emplace_back();

            for (GLint i = 0; i < uniformCount; ++i)
            {
                glGetActiveUniform(programId, i, maxNameLen, &length, &count, &type, nameBuffer.data());

                auto uniformName = std::string(nameBuffer.begin(), nameBuffer.begin() + length);

                reflectionData.buffers[0].addUniform(uniformName);
            }
        }*/

        spv_reflect::ShaderModule module(shader->getCode());
        if (module.GetResult() != SPV_REFLECT_RESULT_SUCCESS)
        {
            CORE_LOG_ERROR("Failed to reflect shader!");
            return {};
        }

        SpvReflectResult result{};

        u32 count;
        result = module.EnumerateDescriptorSets(&count, nullptr);
        RUNE_ENG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS, "");

        std::vector<SpvReflectDescriptorSet*> sets(count);
        result = module.EnumerateDescriptorSets(&count, sets.data());
        RUNE_ENG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS, "");

        for (const auto& reflectedSet : sets)
        {
            auto& set = reflectionData.sets.emplace_back();
            set.set = reflectedSet->set;

            set.bindings.resize(reflectedSet->binding_count);
            for (size bindingIndex = 0; bindingIndex < reflectedSet->binding_count; ++bindingIndex)
            {
                auto& reflectedBinding = reflectedSet->bindings[bindingIndex];

                auto& binding = set.bindings[reflectedBinding->binding];
                binding.set = reflectedBinding->set;
                binding.binding = reflectedBinding->binding;
                binding.name = reflectedBinding->name;
                binding.type = toUniformType(reflectedBinding->descriptor_type);

                binding.bufferSize = reflectedBinding->block.size;
                binding.bufferMembers.resize(reflectedBinding->block.member_count);
                for (size memberIndex = 0; memberIndex < binding.bufferMembers.size(); ++memberIndex)
                {
                    auto& reflectedMember = reflectedBinding->block.members[memberIndex];

                    auto& member = binding.bufferMembers[memberIndex];
                    member.name = reflectedMember.name;
                    member.byteOffset = reflectedMember.offset;
                    member.byteSize = reflectedMember.size;
                }
            }
        }

        return reflectionData;
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

    auto Renderer_OpenGL::createTexture(const Texture* texture) -> GlTexture
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

        return GlTexture{ textureId };
    }

    void Renderer_OpenGL::destroyTexture(const Renderer_OpenGL::GlTexture& glTexture)
    {
        glDeleteTextures(1, &glTexture.id);
    }
}
