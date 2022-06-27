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

        auto createBuffer(size initialSize, void* initialData) -> u32 override;
        auto createTexture() -> void* override;

        void beginFrame() override;
        void endFrame() override;

        void draw() override;
    };
}
