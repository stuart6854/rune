project "Rune-ScriptCore"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"

    targetdir("%{wks.location}/sandbox/assets/scripts")
    objdir("%{wks.location}/sandbox/assets/scripts/intermediates")

    files
    {
        "Source/**.cs"
    }

    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"

    filter "configurations:Dist"
        optimize "Full"
        symbols "Off"