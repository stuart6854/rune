#include "rune/graphics/graphics.hpp"

#include "platform/opengl/renderer.hpp"

namespace Rune
{
    void GraphicsSystem::initRendererFactories()
    {
        registerRendererFactory(RenderingApi::eOpenGL, Renderer_OpenGL::create);
    }
}
