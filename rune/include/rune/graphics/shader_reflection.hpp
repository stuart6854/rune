// # Copyright � Stuart Millman <stu.millman15@gmail.com>

#pragma once
#include "rune/defines.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace Rune
{
    class Shader;

    struct Member
    {
        std::string name;
        u32 byteOffset;
        u32 byteSize;
    };

    enum class BindingType : u8
    {
        eNone,
        eUniformBuffer,
        eTexture
    };

    struct Binding
    {
        u32 set;
        u32 binding;
        std::string name;
        BindingType type;

        size bufferSize;
        std::vector<Member> bufferMembers;
    };

    struct Set
    {
        u32 set;
        std::vector<Binding> bindings;
    };

    struct ReflectionData
    {
        std::vector<Set> sets;
    };

    namespace ShaderReflection
    {
        auto reflect(const Shader* shader) -> ReflectionData;
    }

}
