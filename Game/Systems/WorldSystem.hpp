#pragma once
#include <entt.hpp>
#include <glm/glm.hpp>
#include "Voxel/ChunkManager.hpp"


class WorldSystem {
public:
    explicit WorldSystem(const std::string& saveFolder);
    void update(entt::registry& reg, const glm::vec3& playerPos);
    ChunkManager& chunks() { return m_chunks; }
private:
    ChunkManager m_chunks;
};