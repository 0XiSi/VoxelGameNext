#include "Chunk.hpp"
#include <algorithm>
#include <fstream>

Chunk::Chunk(const glm::ivec2& worldPos) : m_worldPos(worldPos) {}

BlockType Chunk::get(int x, int y, int z) const {
    if (x < 0 || x >= SIZE_X ||
        y < 0 || y >= SIZE_Y ||
        z < 0 || z >= SIZE_Z) return BlockType::AIR;
    return m_blocks[x + z * SIZE_X + y * SIZE_X * SIZE_Z];
}
void Chunk::set(int x, int y, int z, BlockType t) {
    if (x < 0 || x >= SIZE_X ||
        y < 0 || y >= SIZE_Y ||
        z < 0 || z >= SIZE_Z) return;
    m_blocks[x + z * SIZE_X + y * SIZE_X * SIZE_Z] = t;
}

// ---------- Serialization ----------
void Chunk::serialize(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&m_blocks[0]), m_blocks.size());
}
Chunk Chunk::deserialize(std::istream& in, const glm::ivec2& pos) {
    Chunk c(pos);
    in.read(reinterpret_cast<char*>(&c.m_blocks[0]), c.m_blocks.size());
    return c;
}