// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include <mono/metadata/object-forward.h>
#include <mono/metadata/image.h>

#include <string>

namespace Rune
{
    class ScriptEngine
    {
    public:
        static auto getInstance() -> ScriptEngine&;

        void init() const;
        void shutdown() const;

        void loadAssembly(const std::string& filename) const;

        bool entityClassExists(const std::string& fullClassName) const;

        // TODO: Pass Entity as only param
        void onCreateEntity(const std::string& fullClassName) const;
        void onUpdateEntity(float deltaTime) const;

    private:
        friend class ScriptClass;

        void initMono() const;
        void shutdownMono() const;

        MonoObject* instantiateClass(MonoClass* monoClass) const;
        void loadAssemblyClasses(MonoAssembly* assembly) const;
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

        MonoClass* m_monoClass = nullptr;
    };

    class ScriptInstance
    {
    public:
        ScriptInstance(ScriptClass& scriptClass);

        void invokeOnCreate() const;
        void invokeOnUpdate(float deltaTime) const;

    private:
        ScriptClass& m_scriptClass;

        MonoObject* m_instance = nullptr;
        MonoMethod* m_constructor = nullptr;
        MonoMethod* m_onCreateMethod = nullptr;
        MonoMethod* m_onUpdateMethod = nullptr;
    };

    // TODO: Script Component to be moved out to components class
    struct ScriptComponent
    {
        std::string name;

        ScriptComponent() = default;
        ScriptComponent(const ScriptComponent&) = default;
    };
}
