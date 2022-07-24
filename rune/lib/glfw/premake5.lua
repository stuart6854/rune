-- Declare Library
function UseGLFW()
    defines {}
    libdirs { "rune/lib/glfw/lib" }
    links { "glfw3.lib" }
    externalincludedirs { "rune/lib/glfw/include" }
end
