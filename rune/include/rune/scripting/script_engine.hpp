#pragma once

namespace Rune
{
    class ScriptEngine
    {
    public:
        static auto getInstance() -> ScriptEngine&;

        void init();
        void shutdown();

    private:
        void initMono();
        void shutdownMono();
    };
}