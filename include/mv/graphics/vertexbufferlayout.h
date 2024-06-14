#include "vertexbuffer.h"
#include <epoxy/gl.h>
#include <vector>

#pragma once

namespace mv {
struct VertexBufferElement {
    GLuint count;
    GLenum type;
    GLboolean normalized;
};

class VertexBufferLayout {
    std::vector<VertexBufferElement> elements;
    GLuint stride;

  public:
    void push_float(int count);
    void push_uint(int count);
    void push_uchar(int count);
};
} // namespace mv
