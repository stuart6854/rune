#include "pch.hpp"
#include "rune/scripting/script_glue.hpp"

#include "rune/macros.hpp"
#include "rune/utility/guid.hpp"

#include <glm/glm.hpp>

#include <mono/metadata/loader.h>
#include <mono/metadata/object.h>

#include <string>

namespace Rune
{

#define RUNE_ADD_INTERNAL_CALL(FUNC_NAME) mono_add_internal_call("Rune.InternalCalls::" #FUNC_NAME, FUNC_NAME)

    static void NativeLog(MonoString* string, int value)
    {
        char* cStr = mono_string_to_utf8(string);
        std::string str = cStr;
        mono_free(cStr);
        LOG_INFO("{}: {}", str, value);
    }

    static void NativeLog_Vector3(glm::vec3* param, glm::vec3* outResult)
    {
        LOG_INFO("Value: {},{},{}", param->x, param->y, param->z);
        *outResult = glm::normalize(*param);
    }

    static float NativeLog_Vector3Dot(glm::vec3* param)
    {
        LOG_INFO("Value: {},{},{}", param->x, param->y, param->z);
        return glm::dot(*param, *param);
    }

    static void Entity_GetTranslation(Guid entityId, glm::vec3* outTranslation)
    {
        // Scene* scene = ScriptEngine::getSceneContext();
        //Entity entity = scene->getEntityByGuid(entityId);
    }

    static void Entity_SetTranslation(Guid entityId, glm::vec3* translation)
    {
        // Scene* scene = ScriptEngine::getSceneContext();
    }

    void ScriptGlue::registerFunctions()
    {
        RUNE_ADD_INTERNAL_CALL(NativeLog);
        RUNE_ADD_INTERNAL_CALL(NativeLog_Vector3);
        RUNE_ADD_INTERNAL_CALL(NativeLog_Vector3Dot);
        RUNE_ADD_INTERNAL_CALL(Entity_GetTranslation);
        RUNE_ADD_INTERNAL_CALL(Entity_SetTranslation);
    }
}
