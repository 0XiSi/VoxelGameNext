#pragma once
#include "Chunk.hpp"
#include <unordered_map>
#include <algorithm>
#include <glm/glm.hpp>
#include <filesystem>
#include <fstream>
#include "Core/Hash.hpp"

class ChunkManager {
public:
    Chunk* getChunk(const glm::ivec2& chunkPos);
    void    setBlock(int x, int y, int z, BlockType t);
    BlockType getBlock(int x, int y, int z) const;

    // World distance based load/unload
    void updateAround(const glm::vec3& playerWorldPos, int radius);

    // Disk persistence
    void saveAll(const std::string& folder);
    void loadAll(const std::string& folder);

private:
    glm::ivec2  toChunkPos(const glm::vec3& world) const;
    glm::ivec3  toLocal(const glm::vec3& world) const;

    std::unordered_map<glm::ivec2, Chunk, IVec2Hash> m_chunks;
};