#include "renderer_opengl.hpp"

#include "rune/macros.hpp"
#include "rune/systems/window.hpp"
#include "rune/systems/graphics/vertex.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const auto vert_shader_src = R"(
#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_norm;

layout(location = 0) out vec2 out_uv;

void main()
{
	out_uv = a_uv;
	gl_Position = vec4(a_pos, 1.0);
}
)";

const auto frag_shader_src = R"(
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

Rune::Vertex vertices[] = {
    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } },
    { { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } },
    { { 0.0f, 0.5f, 0.0f }, { 0.5f, 1.0f } },
};

u32 vao = 0;
u32 vbo = 0;
u32 ebo = 0;

u32 shaderProgram = 0;

namespace Rune
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
        glad_glGetProgramiv(program, GL_LINK_STATUS, &success);
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

    auto RendererOpenGl::create() -> Owned<RendererBase>
    {
        return CreateOwned<RendererOpenGl>();
    }

    void RendererOpenGl::init()
    {
        bool isOpenGlLoaded = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        RUNE_ENG_ASSERT(isOpenGlLoaded, "Failed to load OpenGL!");

        CORE_LOG_INFO("OpenGL Version {}.{}", GLVersion.major, GLVersion.minor);

#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(openglMessageCallback, nullptr);
#endif

        {
            // TODO: TEST CODE TO BE REMOVED

            glCreateVertexArrays(1, &vao);
            vbo = createBuffer(sizeof(vertices), vertices);
            // ebo = createBuffer(sizeof(vertices), vertices);

            glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));

            glEnableVertexArrayAttrib(vao, 0);
            glVertexArrayAttribBinding(vao, 0, 0);
            glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));

            glEnableVertexArrayAttrib(vao, 1);
            glVertexArrayAttribBinding(vao, 1, 0);
            glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

            glEnableVertexArrayAttrib(vao, 2);
            glVertexArrayAttribBinding(vao, 2, 0);
            glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, norm));

            u32 vertShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertShader, 1, &vert_shader_src, nullptr);
            glCompileShader(vertShader);
            checkForShaderError(vertShader);

            u32 fragShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragShader, 1, &frag_shader_src, nullptr);
            glCompileShader(fragShader);
            checkForShaderError(fragShader);

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertShader);
            glAttachShader(shaderProgram, fragShader);
            glLinkProgram(shaderProgram);
            checkForProgramError(shaderProgram);

            glDeleteShader(vertShader);
            glDeleteShader(fragShader);
        }
    }

    void RendererOpenGl::cleanup() {}

    void RendererOpenGl::setWindow(WindowSystem* window)
    {
        if (window == nullptr)
            return;

        glViewport(0, 0, window->getWidth(), window->getHeight());
    }

    void RendererOpenGl::onFramebufferSize(const i32 width, const i32 height)
    {
        glViewport(0, 0, width, height);
    }

    auto RendererOpenGl::createBuffer(const size initialSize, void* initialData) -> u32
    {
        u32 buffer;
        glCreateBuffers(1, &buffer);

        glNamedBufferData(buffer, initialSize, initialData, GL_STATIC_DRAW);

        return buffer;
    }

    auto RendererOpenGl::createTexture(const i32 width, const i32 height, const TextureFormat format, const void* data) -> u32
    {
        GLuint textureId;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureId);

        glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        auto glInternalFormat = toGLInternalTextureFormat(format);
        glTextureStorage2D(textureId, 1, glInternalFormat, width, height);
        auto glFormat = toGLTextureFormat(format);
        glTextureSubImage2D(textureId, 0, 0, 0, width, height, glFormat, GL_UNSIGNED_BYTE, data);

        glGenerateTextureMipmap(textureId);

        return m_textures.add(textureId);
    }

    void RendererOpenGl::destroyTexture(const i32 id)
    {
        if (id == 0)
            return;

        auto textureId = m_textures.get(id);
        m_textures.remove(id);

        glDeleteTextures(1, &textureId);
    }

    void RendererOpenGl::beginFrame()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3912f, 0.5843f, 0.9294f, 1.0f);  // Cornflower Blue
    }

    void RendererOpenGl::endFrame() {}

    void RendererOpenGl::draw()
    {
        glUseProgram(shaderProgram);

        // Texture
        glBindTextureUnit(0, m_textures.get(1));
        glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}
