#include "rune/graphics/material.hpp"

#include "rune/macros.hpp"
#include "rune/graphics/shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#define GET_UNIFORM(type)                                                     \
    const auto* member = getUniformBufferMember(name);                        \
    if (member == nullptr)                                                    \
        return {};                                                            \
                                                                              \
    const auto& uniformBuffer = m_uniformBuffers[member->uniformBufferIndex]; \
    auto value = uniformBuffer.buffer.read<type>(member->byteOffset);         \
    return value

#define SET_UNIFORM(type, value_ptr)                                          \
    const auto* member = getUniformBufferMember(name);                        \
    if (member == nullptr)                                                    \
        return;                                                               \
                                                                              \
    const auto& uniformBuffer = m_uniformBuffers[member->uniformBufferIndex]; \
    uniformBuffer.buffer.write(value_ptr, sizeof(type), member->byteOffset)

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
        GET_UNIFORM(float);
    }

    void Material::setFloat(const std::string& name, const float value) const
    {
        SET_UNIFORM(float, &value);
    }

    auto Material::getMat4(const std::string& name) const -> glm::mat4
    {
        GET_UNIFORM(glm::mat4);
    }

    void Material::setMat4(const std::string& name, const glm::mat4& value) const
    {
        SET_UNIFORM(glm::mat4, glm::value_ptr(value));
    }

    void Material::setTexture(const std::string& name, Texture* texture)
    {
        const auto it = m_textureMap.find(name);
        if (it == m_textureMap.end())
        {
            CORE_LOG_WARN("Uniform texture '{}' does not exist!", name);
            return;
        }

        const auto textureSlot = it->second;

        m_textures[textureSlot].texture = texture;
    }

    auto Material::getUniformBuffers() const -> const std::vector<UniformBuffer>&
    {
        return m_uniformBuffers;
    }

    auto Material::getTextureSlots() const -> const std::vector<Material::TextureSlot>&
    {
        return m_textures;
    }

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
                    uniformBuffer.binding = binding.binding;
                    uniformBuffer.buffer.allocate(binding.bufferSize);

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

                    auto& texture = m_textures.emplace_back();
                    texture.name = binding.name;
                    texture.binding = binding.binding;

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

        initUniforms();
    }

    auto MaterialInst::getMaterial() const -> Material*
    {
        return m_material;
    }

    auto MaterialInst::getInt(const std::string& name) const -> i32
    {
        GET_UNIFORM(i32);
    }

    void MaterialInst::setInt(const std::string& name, const i32 value) const
    {
        SET_UNIFORM(i32, &value);
        onUniformChanged(uniformBuffer.binding, member->byteOffset, member->byteSize);
    }

    auto MaterialInst::getFloat(const std::string& name) const -> float
    {
        GET_UNIFORM(float);
    }

    void MaterialInst::setFloat(const std::string& name, const float value) const
    {
        SET_UNIFORM(float, &value);
        onUniformChanged(uniformBuffer.binding, member->byteOffset, member->byteSize);
        onUniformChanged(uniformBuffer.binding, member->byteOffset, member->byteSize);
    }

    auto MaterialInst::getFloat2(const std::string& name) const -> glm::vec2
    {
        GET_UNIFORM(glm::vec2);
    }

    void MaterialInst::setFloat2(const std::string& name, const glm::vec2& value) const
    {
        SET_UNIFORM(glm::vec2, glm::value_ptr(value));
        onUniformChanged(uniformBuffer.binding, member->byteOffset, member->byteSize);
    }

    auto MaterialInst::getFloat3(const std::string& name) const -> glm::vec3
    {
        GET_UNIFORM(glm::vec3);
    }

    void MaterialInst::setFloat3(const std::string& name, const glm::vec3& value) const
    {
        SET_UNIFORM(glm::vec3, glm::value_ptr(value));
        onUniformChanged(uniformBuffer.binding, member->byteOffset, member->byteSize);
    }

    auto MaterialInst::getFloat4(const std::string& name) const -> glm::vec4
    {
        GET_UNIFORM(glm::vec4);
    }

    void MaterialInst::setFloat4(const std::string& name, const glm::vec4& value) const
    {
        SET_UNIFORM(glm::vec4, glm::value_ptr(value));
        onUniformChanged(uniformBuffer.binding, member->byteOffset, member->byteSize);
    }

    auto MaterialInst::getMat4(const std::string& name) const -> glm::mat4
    {
        GET_UNIFORM(glm::mat4);
    }

    void MaterialInst::setMat4(const std::string& name, const glm::mat4& value) const
    {
        SET_UNIFORM(glm::mat4, glm::value_ptr(value));
        onUniformChanged(uniformBuffer.binding, member->byteOffset, member->byteSize);
    }

    auto MaterialInst::getData(const std::string& name) const -> const void*
    {
        GET_UNIFORM(const void*);
    }

    void MaterialInst::setData(const std::string& name, const i32 size, const void* data) const
    {
        const auto* member = getUniformBufferMember(name);
        if (member == nullptr)
            return;

        RUNE_ENG_ASSERT(size <= member->byteSize, "Uniform write size overflow!");

        const auto& uniformBuffer = m_uniformBuffers[member->uniformBufferIndex];
        uniformBuffer.buffer.write(data, size, member->byteOffset);
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

        m_textures[textureSlot].texture = texture;
    }

    auto MaterialInst::getUniformBuffers() const -> const std::vector<UniformBuffer>&
    {
        return m_uniformBuffers;
    }

    auto MaterialInst::getTextureSlots() const -> const std::vector<MaterialInst::TextureSlot>&
    {
        return m_textures;
    }

    void MaterialInst::attachObserver(Observer* observer)
    {
        m_observers.push_back(observer);
    }

    void MaterialInst::detachObserver(Observer* observer)
    {
        const auto it = std::ranges::find(m_observers, observer);
        if (it != m_observers.end())
            m_observers.erase(it);
    }

    void MaterialInst::initUniforms()
    {
        const auto& reflectionData = m_material->getShader()->getReflectionData();

        for (const auto& set : reflectionData.sets)
        {
            for (const auto& binding : set.bindings)
            {
                if (binding.type == BindingType::eUniformBuffer)
                {
                    // Create buffer
                    u32 uniformBufferIndex = m_uniformBuffers.size();
                    auto& uniformBuffer = m_uniformBuffers.emplace_back();
                    uniformBuffer.binding = binding.binding;
                    uniformBuffer.buffer.allocate(binding.bufferSize);

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

                    auto& texture = m_textures.emplace_back();
                    texture.name = binding.name;
                    texture.binding = binding.binding;

                    m_textureMap[binding.name] = textureIndex;
                }
            }
        }

        // Copy uniform buffers and textures over
        for (size i = 0; i < m_material->getUniformBuffers().size(); ++i)
        {
            m_uniformBuffers[i].buffer = m_material->getUniformBuffers()[i].buffer;
        }

        for (size i = 0; i < m_material->getTextureSlots().size(); ++i)
        {
            m_textures[i].texture = m_material->getTextureSlots()[i].texture;
        }
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

    void MaterialInst::onDestroying() const
    {
        for (const auto& observer : m_observers)
        {
            observer->destroying(this);
        }
    }

    void MaterialInst::onUniformChanged(u32 bufferIndex, u32 offset, u32 size) const
    {
        for (const auto& observer : m_observers)
        {
            observer->uniformChanged(this, bufferIndex, offset, size);
        }
    }
}
