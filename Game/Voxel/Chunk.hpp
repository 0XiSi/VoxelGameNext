#pragma once
#include "BlockTypes.hpp"
#include <array>
#include <glm/glm.hpp>
#include <ostream>

class Chunk {
public:
    static constexpr int SIZE_X = 16;
    static constexpr int SIZE_Y = 128;
    static constexpr int SIZE_Z = 16;

    Chunk(const glm::ivec2& worldPos);

    BlockType  get(int x, int y, int z) const;
    void       set(int x, int y, int z, BlockType t);

    const glm::ivec2& worldPos() const { return m_worldPos; }

    // Serialization helpers
    void        serialize(std::ostream& out)  const;
    static Chunk deserialize(std::istream& in, const glm::ivec2& pos);

private:
    glm::ivec2                                     m_worldPos;
    std::array<BlockType, SIZE_X* SIZE_Y* SIZE_Z>  m_blocks{ BlockType::AIR };
};