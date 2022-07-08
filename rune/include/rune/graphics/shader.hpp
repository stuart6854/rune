#pragma once

#include "shader_reflection.hpp"
#include "rune/assets/asset.hpp"

#include <vector>

namespace Rune
{
    class Shader : public Asset
    {
    public:
        void setIsCompiled(bool isCompiled);
        void setVertexCode(const std::vector<u8>& code);
        void setFragmentCode(const std::vector<u8>& code);
        void reflect();

        auto getVertexCode() const -> const std::vector<u8>&;
        auto getFragmentCode() const -> const std::vector<u8>&;
        auto getReflectionData() const -> const ReflectionData&;

        void setReflectionData(const ReflectionData& reflectionData);

    private:
        bool m_isCompiled;
        std::vector<u8> m_vertexCode;
        std::vector<u8> m_fragmentCode;

        ReflectionData m_reflectionData;
    };
}
