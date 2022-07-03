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

    void Shader::attachObserver(Observer* observer)
    {
        m_observers.push_back(observer);
    }

    void Shader::detachObserver(Observer* observer)
    {
        const auto it = std::ranges::find(m_observers, observer);
        if (it != m_observers.end())
            m_observers.erase(it);
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

    void Shader::onDestroying() const
    {
        for (const auto& observer : m_observers)
        {
            observer->destroying(this);
        }
    }

    void Shader::onChanged() const
    {
        for (const auto& observer : m_observers)
        {
            observer->changed(this);
        }
    }
}
