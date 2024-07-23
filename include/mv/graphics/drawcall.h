#include "../config.h"
#include "../gl.h"
#include "../resource/material.h"
#include "shader.h"
#include "vertex.h"
#include "vertexbuffer.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#pragma once

namespace mv {
struct DrawCall {
    VertexBuffer vbo = VertexBuffer();
    // std::shared_ptr<Shader> shader = nullptr;
    std::shared_ptr<Material> material = nullptr;
    unsigned int active_texture = 0;
    unsigned int vertex_count = 0;
    Vertex vertices[MAX_VERTICES] = {};
};
} // namespace mv
