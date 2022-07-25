// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"

#include <variant>
#include <string>
#include <unordered_map>

constexpr auto UNNAMED_CONFIG_ROOT = "config/";
constexpr auto UNNAMED_ENGINE_CONFIG_FILENAME = "engine.toml";

namespace Rune
{
    enum class VarType : u8
    {
        eInt,
        eFloat,
        eString
    };

    class ConfigVar
    {
    public:
        ConfigVar(std::string name, i64 initialValue = 0);
        ConfigVar(std::string name, f64 initialValue = 0.0);
        ConfigVar(std::string name, const std::string& initialValue = "");

        auto getName() const -> const std::string&;
        auto getType() const -> VarType;

        auto getInt() const -> i64;
        auto getDouble() const -> f64;
        auto getString() -> std::string;

        void setInt(i64 value);
        void setDouble(f64 value);
        void setString(const std::string& value);

    private:
        std::string m_name;
        VarType m_type;
        std::variant<i64, f64, std::string> m_value;
    };

    class ConfigSystem
    {
    public:
        static auto getInstance() -> ConfigSystem&;

        static auto getEngineConfigFilename() -> std::string;

        void init();
        void cleanup();

        void add(const std::string& name, i64 value = 0);
        void add(const std::string& name, f64 value = 0.0);
        void add(const std::string& name, const std::string& value = "");

        auto get(const std::string& name) -> ConfigVar*;

    private:
        std::unordered_map<std::string, Owned<ConfigVar>> m_vars;
    };

    class ConfigParser
    {
    public:
        static void parse(const std::string& filename, ConfigSystem& configSystem);
    };
}
