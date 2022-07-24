-- Declare Library
function UseSPIRVReflect()
    externalincludedirs { "rune/lib/spirv_reflect" }
    links { "SPIRV-Reflect" }
end

project "SPIRV-Reflect"
    kind "StaticLib"
    language "C"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {        
        "spirv_reflect.c",
        "spirv_reflect.cpp",
        "spirv_reflect.h"
    }

    includedirs
    {
        "include"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "on"

    filter "configurations:Release"
        optimize "on"

    filter "configurations:Dist"
        optimize "on"

    filter {}