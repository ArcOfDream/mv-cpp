#include "mv/graphics/vertex.h"
#include "mv/resource/texture.h"
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

#pragma once

namespace mv {

typedef struct {} CRenderable, CCenterTexture;

typedef std::shared_ptr<Texture> CTexture;

typedef glm::vec2 CPosition, CScale, COffset;

typedef struct CAngle {
    float a = 0;
} CAngleRadians, CAngleDegrees;

typedef glm::vec4 CColor;

typedef glm::mat3 CMatrix;

typedef struct CVertexQuad {
    Vertex v[4];
} CVertexQuad;

} // namespace mv
