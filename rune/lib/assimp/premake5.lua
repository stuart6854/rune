-- Declare Library
function UseAssimp()
    defines {}
    externalincludedirs { "rune/lib/assimp/include" }
    libdirs { "rune/lib/assimp/lib" }

    filter "configurations:Debug"
        links { "assimp-vc143-mtd.lib" }

    filter "configurations:Release"
        links { "assimp-vc143-mt.lib" }

    filter "configurations:Dist"
        links { "assimp-vc143-mt.lib" }

    filter {}
end
