workspace "Rune"
    architecture "x64"
    targetdir "build"

    configurations 
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    startproject "Sandbox"

    outputdir = "%{cfg.system}-%{cfg.buildcfg}"

    group "Dependencies"
        -- include "rune/lib/glfw"
        -- include "rune/lib/glad"
        -- include "rune/lib/fmt"
        -- include "rune/lib/assimp"
        -- include "rune/lib/glm"
        -- include "rune/lib/spdlog"
        -- include "rune/lib/spirv_reflect"
        -- include "rune/lib/stb"
        -- include "rune/lib/tomlplusplus"
    group ""

    group "Core"
        project "Rune"
            location "rune"
            kind "StaticLib"
            language "C++"
            cppdialect "C++17"
            staticruntime "off"

            targetdir("bin/" .. outputdir .. "/%{prj.name}")
            objdir("bint-int/" .. outputdir .. "/%{prj.name}")

            pchheader "pch.hpp"
            pchsource "rune/src/pch.cpp"

            files
            {
                "rune/src/**.hpp",
                "rune/src/**.h",
                "rune/src/**.cpp",
                "rune/src/**.c",
                "rune/include/**.hpp",
                "rune/include/**.h"
            }

            includedirs
            {
                "rune/include",
                "rune/src"
            }

            links
            {
                "GLFW",
                "Glad",
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

    group ""