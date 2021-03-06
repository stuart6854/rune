// # Copyright ? Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/assets/asset.hpp"
#include "rune/utility/buffer.hpp"

#include <glm/mat4x4.hpp>

#include <unordered_map>
#include <vector>

namespace Rune
{
    class Texture;
    class Shader;
    class MaterialInst;

    /**
     * Handles the pipeline state
     */
    class Material final : public Asset
    {
    public:
        struct UniformBuffer
        {
            u32 internalId;
            u32 binding;
            Buffer data;
        };

        struct TextureSlot
        {
            std::string name;
            u32 binding;
            Texture* texture;
        };

    public:
        ~Material() override;

        auto getShader() const -> Shader*;
        void setShader(Shader* shader);

        auto getDefaultInstance() const -> MaterialInst*;
        auto createInstance() -> MaterialInst*;

        auto getFloat(const std::string& name) const -> float;
        void setFloat(const std::string& name, float value) const;

        auto getMat4(const std::string& name) const -> glm::mat4;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        void setTexture(const std::string& name, Texture* texture);

        auto getUniformBuffers() const -> const std::vector<UniformBuffer>&;
        auto getTextureSlots() const -> const std::vector<TextureSlot>&;

        auto getId() const -> u32;

    private:
        void initUniforms();

        struct UniformBufferMember
        {
            u32 uniformBufferIndex;
            u32 byteOffset;
            u32 byteSize;
        };

        auto getUniformBufferMember(const std::string& name) const -> const UniformBufferMember*;

    private:
        u32 m_internalId{};

        Shader* m_shader = nullptr;

        bool m_doubleSided = false;
        bool m_depthTest = true;
        bool m_alphaTest = false;

        Owned<MaterialInst> m_defaultInstance = nullptr;
        std::vector<Owned<MaterialInst>> m_instances;

        std::vector<UniformBuffer> m_uniformBuffers;
        std::vector<TextureSlot> m_textures;

        std::unordered_map<std::string, UniformBufferMember> m_uniformMemberMap;
        std::unordered_map<std::string, size> m_textureMap;
    };

    /**
     * Created from a Material instance. Used to set uniforms/textures
     */
    class MaterialInst final : public Asset
    {
    public:
        struct UniformBuffer
        {
            u32 internalId;
            u32 binding;
            Buffer data;
        };

        struct TextureSlot
        {
            std::string name;
            u32 binding;
            Texture* texture;
        };

    public:
        void init(Material* material);

        auto getMaterial() const -> Material*;

        auto getInt(const std::string& name) const -> i32;
        void setInt(const std::string& name, i32 value) const;

        auto getFloat(const std::string& name) const -> float;
        void setFloat(const std::string& name, float value) const;

        auto getFloat2(const std::string& name) const -> glm::vec2;
        void setFloat2(const std::string& name, const glm::vec2& value) const;

        auto getFloat3(const std::string& name) const -> glm::vec3;
        void setFloat3(const std::string& name, const glm::vec3& value) const;

        auto getFloat4(const std::string& name) const -> glm::vec4;
        void setFloat4(const std::string& name, const glm::vec4& value) const;

        auto getMat4(const std::string& name) const -> glm::mat4;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        auto getData(const std::string& name) const -> const void*;
        void setData(const std::string& name, i32 size, const void* data) const;

        void setTexture(const std::string& name, Texture* texture);

        auto getUniformBuffers() const -> const std::vector<UniformBuffer>&;
        auto getTextureSlots() const -> const std::vector<TextureSlot>&;

    private:
        void initUniforms();

        struct UniformBufferMember
        {
            u32 uniformBufferIndex;
            u32 byteOffset;
            u32 byteSize;
        };

        auto getUniformBufferMember(const std::string& name) const -> const UniformBufferMember*;

    private:
        Material* m_material = nullptr;

        std::vector<UniformBuffer> m_uniformBuffers;
        std::vector<TextureSlot> m_textures;

        std::unordered_map<std::string, UniformBufferMember> m_uniformMemberMap;
        std::unordered_map<std::string, size> m_textureMap;
    };
}
