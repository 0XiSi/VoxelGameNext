#pragma once
#include <glm/glm.hpp>
#include <functional>

struct IVec2Hash {
    std::size_t operator()(const glm::ivec2& v) const noexcept {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
    }
};