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
        include "rune/lib/glfw"
        include "rune/lib/glad"
        -- include "rune/lib/fmt"
        include "rune/lib/assimp"
        include "rune/lib/glm"
        include "rune/lib/spdlog"
        include "rune/lib/spirv_reflect"
        include "rune/lib/stb"
        include "rune/lib/tomlplusplus"
    group ""

function UseRune()
    links { "Rune" }
    externalincludedirs 
    {
        "rune/include",
    }

    UseSpdlog()

    filter "system:windows"
        defines { "RUNE_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "RUNE_DEBUG" }

        postbuildcommands 
        {
            '{COPY} "../rune/lib/assimp/bin/assimp-vc143-mtd.dll" "%{cfg.targetdir}"'
        }

    filter "configurations:Release"
        defines { "RUNE_RELEASE" }

        postbuildcommands 
        {
            '{COPY} "../rune/lib/assimp/bin/assimp-vc143-mt.dll" "%{cfg.targetdir}"'
        }

    filter "configurations:Dist"
        defines { "RUNE_DIST" }

        postbuildcommands 
        {
            '{COPY} "../rune/lib/assimp/bin/assimp-vc143-mt.dll" "%{cfg.targetdir}"'
        }

    filter {}
end

    group "Core"
        project "Rune"
            location "rune"
            kind "StaticLib"
            language "C++"
            cppdialect "C++20"
            staticruntime "off"

            targetdir("bin/" .. outputdir .. "/%{prj.name}")
            objdir("bin-int/" .. outputdir .. "/%{prj.name}")

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
            removefiles { "rune/src/platform/**" }

            includedirs
            {
                "rune/include",
                "rune/src"
            }

            UseSpdlog()
            UseGLFW()
            UseGlad()
            UseGLM()
            UseSPIRVReflect()
            UseAssimp()
            UseSTB()
            UseTOMLPP()

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

                files
                {
                    "rune/src/platform/windows/**",
                    "rune/src/platform/opengl/**"
                }

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

    group "Runtime"
        project "Sandbox"
            location "sandbox"
            kind "ConsoleApp"
            language "C++"
            cppdialect "C++20"
            staticruntime "off"

            targetdir("bin/" .. outputdir .. "/%{prj.name}")
            objdir("bin-int/" .. outputdir .. "/%{prj.name}")

            files
            {
                "sandbox/src/**.hpp",
                "sandbox/src/**.h",
                "sandbox/src/**.cpp",
                "sandbox/src/**.c"
            }

            UseRune()

            filter "system:windows"
                systemversion "latest"

            filter "configurations:Debug"
                symbols "on"

            filter "configurations:Release"
                optimize "on"

            filter "configurations:Dist"
                optimize "on"

    group ""