#pragma once
#include <string>

class ChunkManager;

namespace WorldSerializer {
    void save(const ChunkManager& cm, const std::string& folder);
    void load(ChunkManager& cm, const std::string& folder);
}