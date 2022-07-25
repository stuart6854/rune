// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include <rune.hpp>

class Sandbox : public Rune::Game
{
public:
    void init() override
    {
        LOG_TRACE("Game::init()");
    }

    void update() override {}

    void cleanup() override
    {
        LOG_TRACE("Game::cleanup()");
    }

private:
};

auto Rune::createGame() -> Game*
{
    return new Sandbox();
}