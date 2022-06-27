#pragma once

#include <string>

namespace Rune
{
    /**
     * Used as the application entrypoint.
     */
    class Game
    {
    public:
        void init();
        void update();
        void cleanup();

        void sysInit();
        void sysUpdate();
        void sysCleanup();

        void run();

        static void close();

    private:
    };

    inline Game& createGame();
}  // namespace Rune

// Sample platform-specific function
extern std::string platform;