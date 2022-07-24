-- Declare Library
function UseGLFW()
    libdirs { "%{wks.location}/rune/lib/glfw/lib" }
    links { "glfw3.lib" }
    externalincludedirs { "%{wks.location}/rune/lib/glfw/include" }
end
