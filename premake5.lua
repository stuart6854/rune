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
    include "rune/lib/mono"
    include "rune/lib/assimp"
    include "rune/lib/glm"
    include "rune/lib/spdlog"
    include "rune/lib/spirv_reflect"
    include "rune/lib/stb"
    include "rune/lib/tomlplusplus"
group ""

group "Core"
    include "rune"
    include "rune-scriptcore"
group ""

group "Runtime"
    include "sandbox"
group ""