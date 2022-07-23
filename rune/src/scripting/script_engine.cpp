#include "rune/scripting/script_engine.hpp"

#include "rune/macros.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Rune
{
    struct ScriptEngineData
    {
        MonoDomain* rootDomain = nullptr;
        MonoDomain* appDomain = nullptr;

        MonoAssembly* coreAssembly = nullptr;
    };

    static Owned<ScriptEngineData> s_data;

    auto ScriptEngine::getInstance() -> ScriptEngine&
    {
        static ScriptEngine scriptEngine;
        return scriptEngine;
    }

    void ScriptEngine::init()
    {
        s_data = CreateOwned<ScriptEngineData>();

        initMono();
    }

    void ScriptEngine::shutdown()
    {
        shutdownMono();
        s_data = nullptr;
    }

    char* readBytes(const std::string& filename, u32* outSize)
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

    MonoAssembly* loadCSharpAssembly(const std::string& assemblyPath)
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

    void printAssemblyTypes(MonoAssembly* assembly)
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

    void ScriptEngine::initMono()
    {
        mono_set_assemblies_path("mono/lib");

        MonoDomain* rootDomain = mono_jit_init("RuneJITRuntime");
        RUNE_ENG_ASSERT(rootDomain, "Failed to init ScriptEngine!");

        // Store the root domain pointer
        s_data->rootDomain = rootDomain;

        // Create an App Domain
        s_data->appDomain = mono_domain_create_appdomain(const_cast<char*>("RuneScriptRuntime"), nullptr);
        mono_domain_set(s_data->appDomain, true);

        s_data->coreAssembly = loadCSharpAssembly("assets/scripts/Rune-ScriptCore.dll");
        printAssemblyTypes(s_data->coreAssembly);

        MonoImage* assemblyImage = mono_assembly_get_image(s_data->coreAssembly);
        MonoClass* monoClass = mono_class_from_name(assemblyImage, "Rune", "Main");

        // 1. Create an object (and call Ctor)
        MonoObject* objectInstance = mono_object_new(s_data->appDomain, monoClass);
        mono_runtime_object_init(objectInstance);  // Parameter-less Constructor

        // 2. Call function
        MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
        mono_runtime_invoke(printMessageFunc, objectInstance, nullptr, nullptr);

        // 3. Call function with param(s)
        MonoMethod* printIntFunc = mono_class_get_method_from_name(monoClass, "PrintInt", 1);
        int value = 5;
        void* param = &value;
        mono_runtime_invoke(printIntFunc, objectInstance, &param, nullptr);

        MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
        MonoString* monoString = mono_string_new(s_data->appDomain, "Hello World from C++!");
        void* stringParam = monoString;
        mono_runtime_invoke(printCustomMessageFunc, objectInstance, &stringParam, nullptr);
    }

    void ScriptEngine::shutdownMono()
    {
        // mono_domain_unload(s_data->appDomain); // BUG: Breaks
        mono_jit_cleanup(s_data->rootDomain);
    }

}
