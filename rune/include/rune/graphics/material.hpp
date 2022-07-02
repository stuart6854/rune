#pragma once

#include "rune/assets/asset.hpp"
#include "rune/utility/buffer.hpp"

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
        ~Material() override;

        auto getShader() const -> Shader*;
        void setShader(Shader* shader);

        auto getDefaultInstance() const -> MaterialInst*;
        auto createInstance() -> MaterialInst*;
        
        auto getFloat(const std::string& name) const -> float;
        void setFloat(const std::string& name, float value) const;

        void setTexture(const std::string& name, Texture* texture);

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

        std::vector<Buffer> m_uniformBuffers;
        std::vector<Texture*> m_textures;

        std::unordered_map<std::string, UniformBufferMember> m_uniformMemberMap;
        std::unordered_map<std::string, size> m_textureMap;
    };

    /**
     * Created from a Material instance. Used to set uniforms/textures
     */
    class MaterialInst final : Asset
    {
    public:
        void init(Material* material);

        auto getFloat(const std::string& name) const -> float;
        void setFloat(const std::string& name, float value) const;

        void setTexture(const std::string& name, Texture* texture);

    private:
        struct UniformBufferMember
        {
            u32 uniformBufferIndex;
            u32 byteOffset;
            u32 byteSize;
        };

        auto getUniformBufferMember(const std::string& name) const -> const UniformBufferMember*;

    private:
        Material* m_material = nullptr;

        std::vector<Buffer> m_uniformBuffers;
        std::vector<Texture*> m_textures;

        std::unordered_map<std::string, UniformBufferMember> m_uniformMemberMap;
        std::unordered_map<std::string, size> m_textureMap;
    };
}
