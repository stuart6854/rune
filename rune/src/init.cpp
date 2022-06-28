#include "rune/init.hpp"

#include "rune/macros.hpp"
#include "rune/systems/config.hpp"
#include "rune/systems/graphics/graphics.hpp"
#include "rune/systems/log.hpp"
#include "rune/systems/window.hpp"
#include "rune/systems/assets/asset_factory.hpp"
#include "rune/systems/events/events.hpp"
#include "rune/systems/assets/asset_registry.hpp"

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
        /* Initialise engine subsystems */
        LogSystem::init();

        CORE_LOG_INFO("CD:{}", std::filesystem::current_path().string());
        auto& configInst = ConfigSystem::getInstance();
        configInst.init();
        ConfigParser::parse(ConfigSystem::getEngineConfigFilename(), ConfigSystem::getInstance());

        // TODO: Get window settings from config
        auto windowWidth = configInst.get("rendering.resolution_w");
        WindowSystem::WindowProps props{ "Rune" };
        if (windowWidth)
            props.width = windowWidth->getInt();
        auto windowHeight = configInst.get("rendering.resolution_h");
        if (windowHeight)
            props.height = windowHeight->getInt();
        auto windowVSync = configInst.get("rendering.vsync");
        if (windowVSync)
            props.vSync = windowVSync->getInt();
        auto windowMode = configInst.get("rendering.window_mode");
        if (windowVSync)
            props.windowMode = static_cast<WindowMode>(windowMode->getInt());

        auto& windowInst = WindowSystem::getInstance();
        windowInst.init();
        windowInst.createWindow(props);

        // TODO: Get rendering settings from config
        auto& graphicsInst = GraphicsSystem::getInstance();
        graphicsInst.init(RenderingApi::eOpenGL);
        graphicsInst.setWindow(&WindowSystem::getInstance());

        auto& assetRegistry = AssetRegistry::getInstance();
        assetRegistry.registerFactory<TextureFactory>(AssetType::eTexture);

        auto textureHandle = assetRegistry.add("assets/textures/texture.jpg");
        assetRegistry.load(textureHandle);

        // Register core events
        EventSystem::listen<EventWindowClose>([](const EventWindowClose& event) { Game::close(); });

        /* Call application init */
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
