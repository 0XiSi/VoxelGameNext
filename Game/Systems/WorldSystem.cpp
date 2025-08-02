#include "WorldSystem.hpp"
#include <entt.hpp>

WorldSystem::WorldSystem(const std::string& saveFolder) {
    m_chunks.loadAll(saveFolder);
}

void WorldSystem::update(entt::registry& /*reg*/, const glm::vec3& playerPos) {
    m_chunks.updateAround(playerPos, 8);
}