#pragma once
#include <cstdint>

enum class BlockType : uint8_t {
    AIR = 0,
    STONE = 1,
    GRASS = 2,
    DIRT = 3,
    GRAVEL = 4
};

inline bool isSolid(BlockType t) { return t != BlockType::AIR; }