#include "rune/graphics/material.hpp"

#include "rune/macros.hpp"
#include "rune/graphics/shader.hpp"

namespace Rune
{
    Material::~Material()
    {
        // Kill default instance
        m_defaultInstance.reset();

        // Kill instances of this material
        for (auto& instance : m_instances)
        {
            instance.reset();
        }
    }

    auto Material::getShader() const -> Shader*
    {
        return m_shader;
    }

    void Material::setShader(Shader* shader)
    {
        m_shader = shader;
        // Setup uniform buffers/textures
        initUniforms();

        // Create default instance
        m_defaultInstance = CreateOwned<MaterialInst>();
        m_defaultInstance->init(this);
    }

    auto Material::getDefaultInstance() const -> MaterialInst*
    {
        return m_defaultInstance.get();
    }

    auto Material::createInstance() -> MaterialInst*
    {
        m_instances.push_back(CreateOwned<MaterialInst>());
        m_instances.back()->init(this);
        return m_instances.back().get();
    }

    auto Material::getFloat(const std::string& name) const -> float
    {
        const auto* member = getUniformBufferMember(name);
        if (member == nullptr)
            return 0;

        const auto& uniformBuffer = m_uniformBuffers[member->uniformBufferIndex];
        auto value = uniformBuffer.read<float>(member->byteOffset);
        return value;
    }

    void Material::setFloat(const std::string& name, const float value) const
    {
        const auto* member = getUniformBufferMember(name);
        if (member == nullptr)
            return;

        const auto& uniformBuffer = m_uniformBuffers[member->uniformBufferIndex];
        uniformBuffer.write(&value, sizeof(float), member->byteOffset);
    }

    void Material::setTexture(const std::string& name, Texture* texture) {}

    void Material::initUniforms()
    {
        const auto& reflectionData = m_shader->getReflectionData();

        for (const auto& set : reflectionData.sets)
        {
            for (const auto& binding : set.bindings)
            {
                if (binding.type == BindingType::eUniformBuffer)
                {
                    // Create buffer
                    u32 uniformBufferIndex = m_uniformBuffers.size();
                    auto& uniformBuffer = m_uniformBuffers.emplace_back();
                    uniformBuffer.allocate(binding.bufferSize);

                    for (const auto& bufferMember : binding.bufferMembers)
                    {
                        auto memberName = binding.name + "." + bufferMember.name;

                        m_uniformMemberMap[memberName] = {
                            uniformBufferIndex,
                            bufferMember.byteOffset,
                            bufferMember.byteSize,
                        };
                    }
                }
                else if (binding.type == BindingType::eTexture)
                {
                    auto textureIndex = m_textures.size();
                    m_textures.emplace_back();
                    m_textureMap[binding.name] = textureIndex;
                }
            }
        }
    }

    auto Material::getUniformBufferMember(const std::string& name) const -> const UniformBufferMember*
    {
        const auto it = m_uniformMemberMap.find(name);
        if (it == m_uniformMemberMap.end())
        {
            CORE_LOG_WARN("Uniform member '{}' does not exist!", name);
            return nullptr;
        }

        return &it->second;
    }

    void MaterialInst::init(Material* material)
    {
        m_material = material;

        // Get default uniforms/textures from material
    }

    auto MaterialInst::getFloat(const std::string& name) const -> float
    {
        const auto* member = getUniformBufferMember(name);
        if (member == nullptr)
            return 0;

        const auto& uniformBuffer = m_uniformBuffers[member->uniformBufferIndex];
        auto value = uniformBuffer.read<float>(member->byteOffset);
        return value;
    }

    void MaterialInst::setFloat(const std::string& name, const float value) const
    {
        const auto* member = getUniformBufferMember(name);
        if (member == nullptr)
            return;

        const auto& uniformBuffer = m_uniformBuffers[member->uniformBufferIndex];
        uniformBuffer.write(&value, sizeof(float), member->byteOffset);
    }

    void MaterialInst::setTexture(const std::string& name, Texture* texture)
    {
        const auto it = m_textureMap.find(name);
        if (it == m_textureMap.end())
        {
            CORE_LOG_WARN("Uniform texture '{}' does not exist!", name);
            return;
        }

        const auto textureSlot = it->second;

        m_textures[textureSlot] = texture;
    }

    auto MaterialInst::getUniformBufferMember(const std::string& name) const -> const MaterialInst::UniformBufferMember*
    {
        const auto it = m_uniformMemberMap.find(name);
        if (it == m_uniformMemberMap.end())
        {
            CORE_LOG_WARN("Uniform member '{}' does not exist!", name);
            return nullptr;
        }

        return &it->second;
    }
}
