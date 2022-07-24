-- Declare Library
function UseMono()
    externalincludedirs { "%{wks.location}/rune/lib/mono/include" }
    links { "mono-2.0-sgen.lib" }

    filter "configurations:Debug"
        libdirs { "%{wks.location}/rune/lib/mono/lib/Debug" }

    filter "configurations:Release"
        libdirs { "%{wks.location}/rune/lib/mono/lib/Release" }

    filter "configurations:Dist"
        libdirs { "%{wks.location}/rune/lib/mono/lib/Release" }

    filter {}
end
