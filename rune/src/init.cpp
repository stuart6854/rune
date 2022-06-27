#include "rune/init.hpp"

#include "rune/macros.hpp"
#include "rune/systems/config.hpp"
#include "rune/systems/graphics/graphics.hpp"
#include "rune/systems/log.hpp"
#include "rune/systems/window.hpp"

#include <iostream>
#include <filesystem>

namespace Rune
{

    namespace
    {
        bool s_shouldStop = false;

        // Static engine systems
        // static WindowSystem s_windowSystem;
        // InputSystem& s_inputSystem = InputSystem::getInstance();
    }  // namespace

    void Game::sysInit()
    {
        // Initialise engine subsystems
        LogSystem::init();

        CORE_LOG_INFO("CD:{}", std::filesystem::current_path().string());
        ConfigSystem::getInstance().init();
        ConfigParser::parse(ConfigSystem::getEngineConfigFilename(), ConfigSystem::getInstance());

        WindowSystem::getInstance().init();
        WindowSystem::getInstance().createWindow({ "Rune" });

        // TODO: Get rendering system from config
        GraphicsSystem::getInstance().init(RenderingApi::eOpenGL);
        GraphicsSystem::getInstance().setWindow(&WindowSystem::getInstance());

        // Call application init
        init();
    }

    void Game::sysUpdate() {}

    void Game::sysCleanup()
    {
        // Call application cleanup
        cleanup();

        // Cleanup engine subsystems
        GraphicsSystem::getInstance().cleanup();
        WindowSystem::getInstance().cleanup();
        ConfigSystem::getInstance().cleanup();
        LogSystem::cleanup();
    }

    void Game::run()
    {
        while (!s_shouldStop)
        {
            WindowSystem::getInstance().update();
            sysUpdate();
            update();
            GraphicsSystem::getInstance().render();
        }
    }

    void Game::close()
    {
        s_shouldStop = true;
    }

    Game& createGame()
    {
        static Game g;
        return g;
    }

}  // namespace Rune

int main(int, char**)
{
    std::cout << "Platform: " << platform << std::endl;

    auto& game = Rune::createGame();
    game.sysInit();
    game.run();
    game.sysCleanup();
    return 0;
}
