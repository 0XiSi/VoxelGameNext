#include <entt.hpp>
#include <glad/glad.h>
#include <Render/Camera.hpp>


class RenderSystem {
public:
    void draw(entt::registry& reg, const Camera& cam) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* iterate registry.view<Transform,MeshId>() and draw */
    }
};