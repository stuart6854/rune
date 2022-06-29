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
        virtual ~Game() = default;

        virtual void init() {}
        virtual void update() {}
        virtual void cleanup() {}

        void sysInit();
        void sysUpdate();
        void sysCleanup();

        void run();

        static void close();

    private:
    };

    /* Entry-Point function defined by client. Should return a derived instance of Game */
    extern auto createGame() -> Game*;

}  // namespace Rune

// Sample platform-specific function
extern std::string platform;