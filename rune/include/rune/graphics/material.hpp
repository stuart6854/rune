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
            u32 binding;
            Buffer buffer;
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
        class Observer
        {
        public:
            virtual ~Observer() = default;

            virtual void destroying(const MaterialInst* materialInst) = 0;
            virtual void uniformChanged(const MaterialInst* materialInst, u32 bufferIndex, u32 offset, u32 size) = 0;
        };

        struct UniformBuffer
        {
            u32 binding;
            Buffer buffer;
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

        auto getFloat(const std::string& name) const -> float;
        void setFloat(const std::string& name, float value) const;

        auto getMat4(const std::string& name) const -> glm::mat4;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        void setTexture(const std::string& name, Texture* texture);

        auto getUniformBuffers() const -> const std::vector<UniformBuffer>&;
        auto getTextureSlots() const -> const std::vector<TextureSlot>&;

        void attachObserver(Observer* observer);
        void detachObserver(Observer* observer);

    private:
        void initUniforms();

        struct UniformBufferMember
        {
            u32 uniformBufferIndex;
            u32 byteOffset;
            u32 byteSize;
        };

        auto getUniformBufferMember(const std::string& name) const -> const UniformBufferMember*;

        void onDestroying() const;
        void onUniformChanged(u32 bufferIndex, u32 offset, u32 size) const;

    private:
        std::vector<Observer*> m_observers;

        Material* m_material = nullptr;

        std::vector<UniformBuffer> m_uniformBuffers;
        std::vector<TextureSlot> m_textures;

        std::unordered_map<std::string, UniformBufferMember> m_uniformMemberMap;
        std::unordered_map<std::string, size> m_textureMap;
    };
}
