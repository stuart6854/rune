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
#include "rune/graphics/material.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

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
        Shader* shader = nullptr;
        Material* material = nullptr;

        float rotY = 0;
        glm::mat4 proj;
        glm::mat4 view;
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

        // TODO: AssetRegistry::getInstance().cleanup();

        auto& assetRegistry = AssetRegistry::getInstance();
        assetRegistry.registerFactory<TextureFactory>(AssetType::eTexture);
        assetRegistry.registerFactory<MeshFactory>(AssetType::eMesh);
        assetRegistry.registerFactory<ShaderFactory>(AssetType::eShader);

        // auto textureHandle = assetRegistry.add("assets/textures/texture.jpg");
        auto textureHandle = assetRegistry.add("assets/models/backpack/diffuse.jpg");
        assetRegistry.load(textureHandle);
        texture = assetRegistry.get<Texture>(textureHandle);

        // auto meshHandle = assetRegistry.add("assets/models/pyramid/pyramid.fbx");
        auto meshHandle = assetRegistry.add("assets/models/backpack/backpack.obj");
        assetRegistry.load(meshHandle);
        mesh = assetRegistry.get<Mesh>(meshHandle);

        auto shaderHandle = assetRegistry.add("assets/default.shader");
        assetRegistry.load(shaderHandle);
        shader = assetRegistry.get<Shader>(shaderHandle);

        auto matHandle = assetRegistry.add("default_mat", CreateOwned<Material>());
        material = assetRegistry.get<Material>(matHandle);
        material->setShader(shader);

        material->setTexture("tex", texture);
        // MVP
        proj = glm::perspective(glm::radians(60.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

        // Register core events
        EventSystem::listen<EventWindowClose>([](const EventWindowClose& event) { Game::close(); });

        /* Call application init */
        init();
    }

    void Game::sysUpdate()
    {
        GraphicsSystem::getInstance().beginScene(proj, view);

        rotY += 5.0f * Time::getDeltaTime();
        auto model = glm::rotate(glm::mat4(1.0f), glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
        GraphicsSystem::getInstance().addRenderable({ mesh, material->getDefaultInstance(), model });
    }

    void Game::sysCleanup()
    {
        // Call application cleanup
        cleanup();

        // Cleanup engine subsystems
        // TODO: AssetRegistry::getInstance().cleanup();
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
            GraphicsSystem::getInstance().render();
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
