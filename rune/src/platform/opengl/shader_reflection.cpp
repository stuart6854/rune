// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "rune/graphics/shader_reflection.hpp"

#include "rune/macros.hpp"
#include "rune/graphics/shader.hpp"

#include <spirv_reflect.h>

namespace Rune
{
    auto toUniformType(const SpvReflectDescriptorType spirvType) -> BindingType
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

    void reflectShader(ReflectionData& reflectionData, const std::vector<u8>& code)
    {
        spv_reflect::ShaderModule module(code);
        if (module.GetResult() != SPV_REFLECT_RESULT_SUCCESS)
        {
            CORE_LOG_ERROR("Failed to reflect shader code!");
            return;
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

            for (size bindingIndex = 0; bindingIndex < reflectedSet->binding_count; ++bindingIndex)
            {
                auto& reflectedBinding = reflectedSet->bindings[bindingIndex];

                if (set.bindings.size() <= reflectedBinding->binding)
                    set.bindings.resize(reflectedBinding->binding + 1);

                auto& binding = set.bindings[reflectedBinding->binding];
                binding.set = reflectedBinding->set;
                binding.binding = reflectedBinding->binding;
                binding.name = reflectedBinding->name;
                binding.type = toUniformType(reflectedBinding->descriptor_type);

                binding.bufferSize = reflectedBinding->block.size;

                if (binding.bufferMembers.size() < reflectedBinding->block.member_count)
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
    }

    auto ShaderReflection::reflect(const Shader* shader) -> ReflectionData
    {
        ReflectionData reflectionData;

        reflectShader(reflectionData, shader->getVertexCode());
        reflectShader(reflectionData, shader->getFragmentCode());

        return reflectionData;
    }
}
