#define GLM_FORCE_PURE

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#pragma once

namespace mv {
    struct Vertex {
        glm::vec2 pos;
        glm::vec2 uv;
        glm::vec4 color;
        // float tex_index;
    };
}