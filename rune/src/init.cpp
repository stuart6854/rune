#include "rune/init.hpp"

#include "rune/macros.hpp"
#include "rune/core/config.hpp"
#include "rune/core/log.hpp"
#include "rune/core/time.hpp"
#include "rune/core/window.hpp"
#include "rune/graphics/graphics.hpp"
#include "rune/assets/asset_factory.hpp"
#include "rune/events/events.hpp"
#include "rune/assets/asset_registry.hpp"

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

        Mesh* mesh = nullptr;
        Texture* texture = nullptr;

    }  // namespace

    void Game::sysInit()
    {
        /* Initialise engine subsystems */
        LogSystem::init();
        Time::beginFrame();

        CORE_LOG_INFO("CD:{}", std::filesystem::current_path().string());
        auto& configInst = ConfigSystem::getInstance();
        configInst.init();
        ConfigParser::parse(ConfigSystem::getEngineConfigFilename(), ConfigSystem::getInstance());

        // TODO: Tidy this away somewhere
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
        
        //TODO: AssetRegistry::getInstance().cleanup();

        auto& assetRegistry = AssetRegistry::getInstance();
        assetRegistry.registerFactory<TextureFactory>(AssetType::eTexture);
        assetRegistry.registerFactory<MeshFactory>(AssetType::eMesh);

        // auto textureHandle = assetRegistry.add("assets/textures/texture.jpg");
        auto textureHandle = assetRegistry.add("assets/models/backpack/diffuse.jpg");
        assetRegistry.load(textureHandle);
        texture = assetRegistry.get<Texture>(textureHandle);

        // auto meshHandle = assetRegistry.add("assets/models/pyramid/pyramid.fbx");
        auto meshHandle = assetRegistry.add("assets/models/backpack/backpack.obj");
        assetRegistry.load(meshHandle);
        mesh = assetRegistry.get<Mesh>(meshHandle);

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
        //TODO: AssetRegistry::getInstance().cleanup();
        GraphicsSystem::getInstance().cleanup();
        WindowSystem::getInstance().cleanup();
        ConfigSystem::getInstance().cleanup();
        LogSystem::cleanup();
    }

    void Game::run()
    {
        while (!s_shouldStop)
        {
            Time::beginFrame();
            WindowSystem::getInstance().update();
            sysUpdate();
            update();
            GraphicsSystem::getInstance().render(mesh, texture);
        }
    }

    void Game::close()
    {
        s_shouldStop = true;
    }

}  // namespace Rune

int main(int, char**)
{
    auto* game = Rune::createGame();
    game->sysInit();
    game->run();
    game->sysCleanup();
    delete game;

    return 0;
}
