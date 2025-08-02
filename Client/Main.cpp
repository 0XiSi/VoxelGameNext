#include <iostream>
#include <entt.hpp>
#include <Systems/WorldSystem.hpp>
#include <Systems/WorldSerializer.hpp>
#include <Platform/Window.hpp>

int main() {
    std::cout << "VoxelGame Client starting...\n";

    // 1. Window & OpenGL context
    Engine::Window window(1280, 720, "VoxelGame");
    if (!window.init()) return -1;

    // 2. ECS world
    entt::registry registry;
    WorldSystem world("save");

    // 3. Dummy camera
    struct Camera { glm::vec3 position{ 0.f, 80.f, 0.f }; } camera;

    // 4. Main loop
    while (!window.shouldClose()) {
        window.pollEvents();

        // TODO plug real input here
        if (window.isKeyPressed(Engine::Key::F5)) {
            WorldSerializer::save(world.chunks(), "save");
            std::cout << "World saved!\n";
        }

        // TODO plug real renderer here
        world.update(registry, camera.position);

        window.swapBuffers();
    }

    return 0;
}