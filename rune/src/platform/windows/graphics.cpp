#include "rune/systems/graphics/graphics.hpp"

#include "renderer_opengl.hpp"

namespace Rune
{
    void GraphicsSystem::initRendererFactories()
    {
        registerRendererFactory(RenderingApi::eOpenGL, RendererOpenGl::create);
    }
}
