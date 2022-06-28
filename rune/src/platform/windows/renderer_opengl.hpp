#pragma once

#include "rune/systems/graphics/graphics.hpp"

namespace Rune
{
    class RendererOpenGl : public RendererBase
    {
    public:
        static auto create() -> Owned<RendererBase>;

        void init() override;
        void cleanup() override;

        void setWindow(WindowSystem* window) override;
        void onFramebufferSize(i32 width, i32 height) override;

        auto createBuffer(size initialSize, void* initialData) -> u32 override;

        auto createTexture(i32 width, i32 height, TextureFormat format, const void* data) -> u32 override;
        void destroyTexture(i32 id) override;

        void beginFrame() override;
        void endFrame() override;

        void draw() override;
    };
}
