#include "rune/graphics/shader.hpp"

#include "rune/graphics/shader_reflection.hpp"

namespace Rune
{
    void Shader::setIsCompiled(const bool isCompiled)
    {
        m_isCompiled = isCompiled;
    }

    void Shader::setVertexCode(const std::vector<u8>& code)
    {
        m_vertexCode = code;
    }

    void Shader::setFragmentCode(const std::vector<u8>& code)
    {
        m_fragmentCode = code;
    }

    void Shader::reflect()
    {
        m_reflectionData = ShaderReflection::reflect(this);
    }

    auto Shader::getVertexCode() const -> const std::vector<u8>&
    {
        return m_vertexCode;
    }

    auto Shader::getFragmentCode() const -> const std::vector<u8>&
    {
        return m_fragmentCode;
    }

    auto Shader::getReflectionData() const -> const ReflectionData&
    {
        return m_reflectionData;
    }

    void Shader::setReflectionData(const ReflectionData& reflectionData)
    {
        m_reflectionData = reflectionData;
    }
}
