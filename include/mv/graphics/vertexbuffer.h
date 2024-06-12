#include <epoxy/gl.h>
#include <epoxy/gl_generated.h>

#pragma once

namespace mv {
class VertexBuffer {
    GLuint id;

  public:
    VertexBuffer();
    VertexBuffer(const void*, GLuint);
    ~VertexBuffer();
    void buffer_data(const void*, GLuint);
    void bind() const;
    void unbind() const;
    GLuint get_id() const;
};
} // namespace mv
