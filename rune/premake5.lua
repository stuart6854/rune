project "Rune"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.hpp"
    pchsource "src/pch.cpp"

    files
    {
        "src/**.hpp",
        "src/**.h",
        "src/**.cpp",
        "src/**.c",
        "include/**.hpp",
        "include/**.h"
    }

    includedirs
    {
        "%{wks.location}/rune/include",
        "%{wks.location}/rune/src"
    }

    UseSpdlog()
    UseGLFW()
    UseGlad()
    UseGLM()
    UseSPIRVReflect()
    UseAssimp()
    UseSTB()
    UseTOMLPP()
    UseEntt()
    UseMono()

    links
    {
        "opengl32.lib"
    }

    defines
    {
        "GLM_FORCE_RADIANS",
        "GLM_FORCE_LEFT_HANDED"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "RUNE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "RUNE_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines
        {
            "RUNE_RELEASE",
            "NDEBUG"
        }

        optimize "on"

    filter "configurations:Dist"
        defines 
        {
            "RUNE_DIST"
        }

        optimize "on"