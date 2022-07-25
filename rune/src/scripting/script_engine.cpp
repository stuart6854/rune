// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "rune/scripting/script_engine.hpp"

#include "rune/macros.hpp"
#include "rune/scripting/script_glue.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Rune
{
    namespace Utils
    {
        // TODO: Move to a File IO class
        static char* readBytes(const std::string& filename, u32* outSize)
        {
            std::ifstream stream(filename, std::ios::binary | std::ios::ate);
            if (!stream)
            {
                CORE_LOG_ERROR("[ScriptEngine] Failed to open file: {}", filename);
                return nullptr;
            }

            auto end = stream.tellg();
            stream.seekg(0, std::ios::beg);

            u32 size = end - stream.tellg();
            if (size == 0)
            {
                CORE_LOG_WARN("[ScriptEngine] File is empty: {}", filename);
                return nullptr;
            }

            auto* buffer = new char[size];
            stream.read(buffer, size);
            stream.close();

            *outSize = size;
            return buffer;
        }

        static MonoAssembly* loadMonoAssembly(const std::string& assemblyPath)
        {
            u32 fileSize = 0;
            char* fileData = readBytes(assemblyPath, &fileSize);

            // Note: We cant use this image for anything other than loading the assembly because this image does not have a reference to the
            // assembly
            MonoImageOpenStatus status;
            MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, true, &status, false);
            if (status != MONO_IMAGE_OK)
            {
                const char* errorMessage = mono_image_strerror(status);
                CORE_LOG_ERROR("[ScriptEngine] Failed to loaded C# assembly: {}\nError: {}", assemblyPath, errorMessage);
                return nullptr;
            }

            MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, false);
            mono_image_close(image);

            // Do not forget to free to file data
            delete[] fileData;

            return assembly;
        }

        static void printAssemblyTypes(MonoAssembly* assembly)
        {
            MonoImage* image = mono_assembly_get_image(assembly);
            const MonoTableInfo* typeDefinitionTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);

            i32 numTypes = mono_table_info_get_rows(typeDefinitionTable);
            for (i32 i = 0; i < numTypes; ++i)
            {
                u32 cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row(typeDefinitionTable, i, cols, MONO_TYPEDEF_SIZE);

                const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
                const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

                CORE_LOG_TRACE("{}.{}", nameSpace, name);
            }
        }
    }

    struct ScriptEngineData
    {
        MonoDomain* rootDomain = nullptr;
        MonoDomain* appDomain = nullptr;

        MonoAssembly* coreAssembly = nullptr;
        MonoImage* coreAssemblyImage = nullptr;

        ScriptClass entityClass;
    };

    static Owned<ScriptEngineData> s_data;

    auto ScriptEngine::getInstance() -> ScriptEngine&
    {
        static ScriptEngine scriptEngine;
        return scriptEngine;
    }

    void ScriptEngine::init() const
    {
        s_data = CreateOwned<ScriptEngineData>();

        initMono();
        loadAssembly("assets/scripts/Rune-ScriptCore.dll");

        ScriptGlue::registerFunctions();

        // Retrieve and instantiate class
        s_data->entityClass = ScriptClass("Rune", "Entity");

        MonoObject* instance = s_data->entityClass.instantiate();

        // Call method
        MonoMethod* printMessageFunc = s_data->entityClass.getMethod("PrintMessage", 0);
        s_data->entityClass.invokeMethod(instance, printMessageFunc);

        // Call method with param(s)
        MonoMethod* printIntFunc = s_data->entityClass.getMethod("PrintInt", 1);
        int value = 5;
        void* param = &value;
        s_data->entityClass.invokeMethod(instance, printIntFunc, &param);

        MonoMethod* printCustomMessageFunc = s_data->entityClass.getMethod("PrintCustomMessage", 1);
        MonoString* monoString = mono_string_new(s_data->appDomain, "Hello World from C++!");
        void* stringParam = monoString;
        s_data->entityClass.invokeMethod(instance, printCustomMessageFunc, &stringParam);
    }

    void ScriptEngine::shutdown() const
    {
        shutdownMono();
        s_data = nullptr;
    }

    void ScriptEngine::initMono() const
    {
        mono_set_assemblies_path("mono/lib");

        MonoDomain* rootDomain = mono_jit_init("RuneJITRuntime");
        RUNE_ENG_ASSERT(rootDomain, "Failed to init ScriptEngine!");

        // Store the root domain pointer
        s_data->rootDomain = rootDomain;
    }

    void ScriptEngine::shutdownMono() const
    {
        // mono_domain_unload(s_data->appDomain); // BUG: Breaks
        mono_jit_cleanup(s_data->rootDomain);
    }

    void ScriptEngine::loadAssembly(const std::string& filename) const
    {
        // Create an App Domain
        s_data->appDomain = mono_domain_create_appdomain(const_cast<char*>("RuneScriptRuntime"), nullptr);
        mono_domain_set(s_data->appDomain, true);

        s_data->coreAssembly = Utils::loadMonoAssembly(filename);
        s_data->coreAssemblyImage = mono_assembly_get_image(s_data->coreAssembly);
        // Utils::printAssemblyTypes(s_data->coreAssembly);
    }

    MonoObject* ScriptEngine::instantiateClass(MonoClass* monoClass) const
    {
        MonoObject* instance = mono_object_new(s_data->appDomain, monoClass);
        mono_runtime_object_init(instance);  // Parameter-less Constructor
        return instance;
    }

    ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
        : m_classNamespace(classNamespace), m_className(className)
    {
        m_monoClass = mono_class_from_name(s_data->coreAssemblyImage, classNamespace.c_str(), className.c_str());
    }

    MonoObject* ScriptClass::instantiate() const
    {
        return ScriptEngine::getInstance().instantiateClass(m_monoClass);
    }

    MonoMethod* ScriptClass::getMethod(const std::string& name, const int paramCount) const
    {
        return mono_class_get_method_from_name(m_monoClass, name.c_str(), paramCount);
    }

    MonoObject* ScriptClass::invokeMethod(MonoObject* instance, MonoMethod* method, void** params) const
    {
        return mono_runtime_invoke(method, instance, params, nullptr);
    }
}
