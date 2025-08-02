#include "ChunkManager.hpp"

Chunk* ChunkManager::getChunk(const glm::ivec2& cp) {
    auto it = m_chunks.find(cp);
    return it != m_chunks.end() ? &it->second : nullptr;
}

BlockType ChunkManager::getBlock(int x, int y, int z) const {
    glm::ivec2 cp = toChunkPos({ x, y, z });
    if (auto* c = const_cast<ChunkManager*>(this)->getChunk(cp))
        return c->get(toLocal({ x, y, z }).x,
            toLocal({ x, y, z }).y,
            toLocal({ x, y, z }).z);
    return BlockType::AIR;
}
void ChunkManager::setBlock(int x, int y, int z, BlockType t) {
    glm::ivec2 cp = toChunkPos({ x, y, z });
    if (!m_chunks.count(cp)) m_chunks.emplace(cp, Chunk{ cp });
    m_chunks.at(cp).set(toLocal({ x, y, z }).x,
        toLocal({ x, y, z }).y,
        toLocal({ x, y, z }).z,
        t);
}

void ChunkManager::updateAround(const glm::vec3& player, int radius) {
    glm::ivec2 center = toChunkPos(player);
    for (int dx = -radius; dx <= radius; ++dx)
        for (int dz = -radius; dz <= radius; ++dz) {
            glm::ivec2 cp = center + glm::ivec2(dx, dz);
            if (!m_chunks.count(cp))
                m_chunks.emplace(cp, Chunk{ cp });
        }

    // unload far chunks
    for (auto it = m_chunks.begin(); it != m_chunks.end();) {
        glm::ivec2 d = it->first - center;
        if (std::max(std::abs(d.x), std::abs(d.y)) > radius)
            it = m_chunks.erase(it);
        else
            ++it;
    }
}

// ---------- Disk ----------
void ChunkManager::saveAll(const std::string& folder) {
    std::filesystem::create_directories(folder);
    for (const auto& [pos, chunk] : m_chunks) {
        std::ofstream out(folder + "/" + std::to_string(pos.x) + "_" + std::to_string(pos.y) + ".chunk", std::ios::binary);
        chunk.serialize(out);
    }
}
void ChunkManager::loadAll(const std::string& folder) {
    if (!std::filesystem::exists(folder)) return;
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        std::ifstream in(entry.path(), std::ios::binary);
        glm::ivec2 pos;
        // parse filename: "x_z.chunk"
        std::string name = entry.path().stem().string();
        sscanf(name.c_str(), "%d_%d", &pos.x, &pos.y);
        m_chunks.emplace(pos, Chunk::deserialize(in, pos));
    }
}

glm::ivec2 ChunkManager::toChunkPos(const glm::vec3& w) const {
    return glm::ivec2(std::floor(w.x / float(Chunk::SIZE_X)),
        std::floor(w.z / float(Chunk::SIZE_Z)));
}
glm::ivec3 ChunkManager::toLocal(const glm::vec3& w) const {
    glm::ivec2 cp = toChunkPos(w);
    return glm::ivec3(int(w.x) - cp.x * Chunk::SIZE_X,
        int(w.y),
        int(w.z) - cp.y * Chunk::SIZE_Z);
}