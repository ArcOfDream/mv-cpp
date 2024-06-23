#include "mv/context.h"
#include "mv/graphics/renderer.h"
#include "mv/graphics/vertex.h"
#include "mv/resource/texture.h"
#include <flecs.h>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

#pragma once

namespace mv {

typedef std::shared_ptr<Renderer> CRenderer;

typedef std::shared_ptr<Context> CContext;

typedef struct CRenderable {
} CRenderable;

typedef std::shared_ptr<Texture> CTexture;

typedef glm::vec2 CPosition, CScale;

typedef struct CAngle {
    float a = 0;
} CAngleRadians, CAngleDegrees;

typedef glm::vec4 CColor;

typedef glm::mat3 CMatrix;

typedef struct CVertexQuad {
    Vertex v[4];
} CVertexQuad;

} // namespace mv
