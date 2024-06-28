#include "../config.h"
#include "mv/gl.h"
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
    std::shared_ptr<Shader> shader = nullptr;
    int textures[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int active_texture = 0;
    int texture_count = 8;
    unsigned int vertex_count = 0;
    Vertex vertices[MAX_VERTICES] = {};
};
} // namespace mv
