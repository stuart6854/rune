project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.hpp",
        "src/**.cpp",
    }

    externalincludedirs
    {
        "%{wks.location}/rune/include",
        "%{wks.location}/rune/lib/spdlog/include"
    }

    links
    {
        "Rune",
        "Rune-ScriptCore"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "RUNE_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "RUNE_DEBUG" }
        runtime "Debug"
        symbols "on"

        postbuildcommands 
        {
            '{COPY} "../rune/lib/assimp/bin/assimp-vc143-mtd.dll" "%{cfg.targetdir}"',
            '{COPY} "../rune/lib/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }

    filter "configurations:Release"
        defines { "RUNE_RELEASE" }
        runtime "Release"
        optimize "on"

        postbuildcommands 
        {
            '{COPY} "../rune/lib/assimp/bin/assimp-vc143-mt.dll" "%{cfg.targetdir}"',
            '{COPY} "../rune/lib/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }

    filter "configurations:Dist"
        defines { "RUNE_RELEASE" }
        runtime "Release"
        optimize "on"

        postbuildcommands 
        {
            '{COPY} "../rune/lib/assimp/bin/assimp-vc143-mt.dll" "%{cfg.targetdir}"',
            '{COPY} "../rune/lib/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
        }