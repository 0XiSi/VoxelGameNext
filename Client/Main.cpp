#include <iostream>
#include <entt.hpp>
#include <Systems/WorldSystem.hpp>
#include <Systems/WorldSerializer.hpp>
#include <Platform/Window.hpp>
#include <Render/Camera.hpp>
#include "Render/RenderSystem.hpp"


int main() {
    std::cout << "VoxelGame Client starting...\n";

    // 1. Window & OpenGL context
    Engine::Window window(1280, 720, "VoxelGame");
    if (!window.init()) return -1;

    // 2. ECS world
    entt::registry registry;
    WorldSystem world("save");

    Camera camera = Camera(glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, 0.0), 0, 0, 5, 5);

    // 4. Main loop
    RenderSystem rs;
    while (!window.shouldClose()) {
        window.pollEvents();
        if (window.isKeyPressed(Engine::Key::F5)) {
            WorldSerializer::save(world.chunks(), "save");
            std::cout << "World saved!\n";
        }

        world.update(registry, camera.getCameraPosition());


        window.swapBuffers();
    }

    return 0;
}