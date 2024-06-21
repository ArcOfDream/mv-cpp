#include "mv/graphics/renderer.h"
#include "mv/graphics/vertex.h"
#include "mv/resource/texture.h"
#include "mv/context.h"
#include <flecs.h>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace mv {

typedef std::shared_ptr<Renderer> CRenderer;

typedef std::shared_ptr<Context> CContext;

typedef struct CRenderable {
} CRenderable;

typedef glm::vec2 CPosition, CScale;

typedef struct CAngle {
    float a = 0;
} CAngleRadians, CAngleDegrees;

typedef glm::vec4 CColor;

typedef struct CTexture {
    std::shared_ptr<Texture> t;
} CTexture;

typedef glm::mat3 CMatrix;

typedef struct CVertexQuad {
    Vertex v[4];
} CVertexQuad;

} // namespace mv

