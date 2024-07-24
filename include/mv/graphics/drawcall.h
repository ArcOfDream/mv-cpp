#include "../config.h"
#include "../gl.h"
#include "vertex.h"
#include "vertexbuffer.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#pragma once

namespace mv {

class Material;

struct DrawCall {
    VertexBuffer vbo = VertexBuffer();
    // std::shared_ptr<Shader> shader = nullptr;
    std::shared_ptr<Material> material = nullptr;
    unsigned int active_texture = 0;
    unsigned int sampler_count = 0;
    unsigned int vertex_count = 0;
    Vertex vertices[MAX_VERTICES] = {};
};
} // namespace mv
