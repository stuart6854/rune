#include "rune/init.hpp"

#include "rune/macros.hpp"
#include "rune/core/config.hpp"
#include "rune/core/log.hpp"
#include "rune/core/time.hpp"
#include "rune/core/window.hpp"
#include "rune/graphics/graphics.hpp"
#include "rune/graphics/material.hpp"
#include "rune/input/input.hpp"
#include "rune/assets/asset_factory.hpp"
#include "rune/assets/asset_registry.hpp"
#include "rune/events/events.hpp"

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
        glm::mat4 projMatrix;
        glm::mat4 viewMatrix;

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

        auto& inputInst = InputSystem::getInstance();
        inputInst.init({ props.width, props.height });

        // TODO: Get rendering settings from config
        auto& graphicsInst = GraphicsSystem::getInstance();
        graphicsInst.init(RenderingApi::eOpenGL);
        graphicsInst.setWindow(&WindowSystem::getInstance());

        AssetRegistry::getInstance().init();

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

        material->getDefaultInstance()->setTexture("tex", texture);
        // MVP
        float aspect = static_cast<float>(props.width) / static_cast<float>(props.height);
        projMatrix = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

        auto cameraPosition = glm::vec3(-5.0f, 5.0f, -5.0f);
        viewMatrix = glm::translate(glm::mat4(1.0f), cameraPosition);
        viewMatrix = glm::rotate(viewMatrix, glm::radians(45.0f), glm::vec3(0, 1, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(30.0f), glm::vec3(1, 0, 0));
        viewMatrix = glm::inverse(viewMatrix);

        // Register core events
        EventSystem::listen<EventWindowClose>([](const EventWindowClose& event) { Game::close(); });

        /* Call application init */
        init();
    }

    void Game::sysUpdate()
    {
        Time::beginFrame();
        InputSystem::getInstance().newFrame();
        WindowSystem::getInstance().update();

        if (InputSystem::getInstance().isKeyDown(Input::KEY_W))
            CORE_LOG_INFO("Key down");
        if (InputSystem::getInstance().isKeyUp(Input::KEY_W))
            CORE_LOG_INFO("Key up");
        if (InputSystem::getInstance().isKeyHeld(Input::KEY_W))
            CORE_LOG_INFO("Key held");

        if (InputSystem::getInstance().isMouseButtonDown(Input::MOUSE_BUTTON_RIGHT))
            CORE_LOG_INFO("Mouse Btn down");
        if (InputSystem::getInstance().isMouseButtonUp(Input::MOUSE_BUTTON_RIGHT))
            CORE_LOG_INFO("Mouse Btn up");
        if (InputSystem::getInstance().isMouseButtonHeld(Input::MOUSE_BUTTON_RIGHT))
            CORE_LOG_INFO("Mouse Btn held");

        /*auto cursorPos = InputSystem::getInstance().getCursorPos();
        CORE_LOG_INFO("{}, {}", cursorPos.x, cursorPos.y);*/

        auto cursorDelta = InputSystem::getInstance().getCursorDelta();
        if (cursorDelta != glm::vec2{})
            CORE_LOG_INFO("{}, {}", cursorDelta.x, cursorDelta.y);

        // Toggle fullscreen
        if (InputSystem::getInstance().isKeyDown(Input::KEY_F11))
        {
            auto mode = WindowSystem::getInstance().getWindowMode();
            if (mode == WindowMode::eWindowed)
                WindowSystem::getInstance().setWindowMode(WindowMode::eFullscreen);
            else
                WindowSystem::getInstance().setWindowMode(WindowMode::eWindowed);
        }

        constexpr float radius = 5.0f;
        auto time = Time::getTimeSinceStartup() * 50.0f;
        glm::vec3 lightPos = { glm::cos(glm::radians(time)) * radius, 5.0f, glm::sin(glm::radians(time)) * radius };
        // glm::vec3 lightPos = { 2.0f, 2.0f, -2.0f };

        Lighting lighting{};
        lighting.viewPos = { 0, 0, -5.0f, 1.0f };
        lighting.ambient = { 0.1f, 0.1f, 0.1f };
        lighting.lightCount = 1;
        // lighting.lights[0].position = { 5, 5, -3 };
        lighting.lights[0].position = lightPos;
        lighting.lights[0].isDirectional = false;
        lighting.lights[0].direction = glm::normalize(glm::vec4{ 0, 0, -1, 1 });
        lighting.lights[0].diffuseColor = { 1.0f, 1.0f, 1.0f, 1 };
        lighting.lights[0].specularColor = { 1, 1, 1, 1 };

        GraphicsSystem::getInstance().beginScene(projMatrix, viewMatrix, lighting);

        rotY += 5.0f * Time::getDeltaTime();

        // Centre
        auto worldMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
        // auto worldMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
        GraphicsSystem::getInstance().addRenderable(worldMatrix, mesh, material->getDefaultInstance());

        // Forward
        worldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4, 0, 0));
        GraphicsSystem::getInstance().addRenderable(worldMatrix, mesh, material->getDefaultInstance());

        // Right
        worldMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 4));
        GraphicsSystem::getInstance().addRenderable(worldMatrix, mesh, material->getDefaultInstance());
    }

    void Game::sysCleanup()
    {
        // Call application cleanup
        cleanup();

        // Cleanup engine subsystems
        AssetRegistry::getInstance().cleanup();
        GraphicsSystem::getInstance().cleanup();
        InputSystem::getInstance().cleanup();
        WindowSystem::getInstance().cleanup();
        ConfigSystem::getInstance().cleanup();
        LogSystem::cleanup();
    }

    void Game::run()
    {
        while (!s_shouldStop)
        {
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
