#include "rune/core/config.hpp"

#include "rune/macros.hpp"

#define TOML_EXCEPTIONS 0
#include <toml++/toml.h>

namespace Rune
{
    ConfigVar::ConfigVar(std::string name, i64 initialValue) : m_name(std::move(name)), m_type(VarType::eInt), m_value(initialValue) {}

    ConfigVar::ConfigVar(std::string name, f64 initialValue) : m_name(std::move(name)), m_type(VarType::eFloat), m_value(initialValue) {}

    ConfigVar::ConfigVar(std::string name, const std::string& initialValue)
        : m_name(std::move(name)), m_type(VarType::eString), m_value(initialValue)
    {
    }

    auto ConfigVar::getName() const -> const std::string&
    {
        return m_name;
    }

    auto ConfigVar::getType() const -> VarType
    {
        return m_type;
    }

    auto ConfigVar::getInt() const -> i64
    {
        if (m_type != VarType::eInt)
        {
            CORE_LOG_WARN("[Config] Trying to get incorrect type from var!");
            return 0;
        }

        return std::get<i64>(m_value);
    }

    auto ConfigVar::getDouble() const -> f64
    {
        if (m_type != VarType::eFloat)
        {
            CORE_LOG_WARN("[Config] Trying to get incorrect type from var!");
            return 0.0;
        }

        return std::get<f64>(m_value);
    }

    auto ConfigVar::getString() -> std::string
    {
        if (m_type != VarType::eString)
        {
            CORE_LOG_WARN("[Config] Trying to get incorrect type from var!");
            return "";
        }

        return std::get<std::string>(m_value);
    }

    void ConfigVar::setInt(i64 value)
    {
        m_value = value;
    }

    void ConfigVar::setDouble(f64 value)
    {
        m_value = value;
    }

    void ConfigVar::setString(const std::string& value)
    {
        m_value = value;
    }

    auto ConfigSystem::getInstance() -> ConfigSystem&
    {
        static ConfigSystem configSystem;
        return configSystem;
    }

    auto ConfigSystem::getEngineConfigFilename() -> std::string
    {
        std::string filename = UNNAMED_CONFIG_ROOT;
        filename.append(UNNAMED_ENGINE_CONFIG_FILENAME);
        return filename;
    }

    void ConfigSystem::init() {}

    void ConfigSystem::cleanup() {}

    void ConfigSystem::add(const std::string& name, i64 value)
    {
        m_vars[name] = CreateOwned<ConfigVar>(name, value);
    }

    void ConfigSystem::add(const std::string& name, f64 value)
    {
        m_vars[name] = CreateOwned<ConfigVar>(name, value);
    }

    void ConfigSystem::add(const std::string& name, const std::string& value)
    {
        m_vars[name] = CreateOwned<ConfigVar>(name, value);
    }

    auto ConfigSystem::get(const std::string& name) -> ConfigVar*
    {
        const auto it = m_vars.find(name);
        if (it == m_vars.end())
            return nullptr;

        return it->second.get();
    }

    void ConfigParser::parse(const std::string& filename, ConfigSystem& configSystem)
    {
        auto result = toml::parse_file(filename);
        if (!result)
        {
            auto err = result.error();
            CORE_LOG_ERROR("Failed to parse config file: {}\n{}", filename, err.description());
            return;
        }

        auto config = result.table();

        for (const auto& value : config)
        {
            const auto sectionName = value.first.str();
            auto* table = value.second.as_table();
            if (table == nullptr)
                continue;

            table->for_each(
                [&configSystem, &sectionName](const toml::key& key, auto&& val)
                {
                    std::string varName = sectionName.data();
                    varName.append(".");
                    varName.append(key.str());

                    if constexpr (toml::is_integer<decltype(val)>)
                    {
                        const auto value = static_cast<i64>(val);
                        CORE_LOG_TRACE("{}={}", varName, value);
                        configSystem.add(varName, value);
                    }
                    if constexpr (toml::is_floating_point<decltype(val)>)
                    {
                        const auto value = static_cast<f64>(val);
                        CORE_LOG_TRACE("{}={}", varName, value);
                        configSystem.add(varName, value);
                    }
                    if constexpr (toml::is_string<decltype(val)>)
                    {
                        const auto value = static_cast<std::string>(val);
                        CORE_LOG_TRACE("{}={}", varName, value);
                        configSystem.add(varName, value);
                    }
                });
        }
    }
}
