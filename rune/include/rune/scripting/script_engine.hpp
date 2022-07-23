#pragma once

#include <mono/metadata/object-forward.h>

#include <string>

namespace Rune
{
    class ScriptEngine
    {
    public:
        static auto getInstance() -> ScriptEngine&;

        void init() const;
        void shutdown() const;

    private:
        friend class ScriptClass;

        void initMono() const;
        void shutdownMono() const;

        void loadAssembly(const std::string& filename) const;

        MonoObject* instantiateClass(MonoClass* monoClass) const;
    };

    class ScriptClass
    {
    public:
        ScriptClass() = default;

        ScriptClass(const std::string& classNamespace, const std::string& className);

        MonoObject* instantiate() const;

        MonoMethod* getMethod(const std::string& name, int paramCount) const;
        MonoObject* invokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr) const;

    private:
        std::string m_classNamespace;
        std::string m_className;

        MonoClass* m_monoClass;
    };
}
