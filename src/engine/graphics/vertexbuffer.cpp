#include "mv/graphics/vertexbuffer.h"
#include "mv/gl.h"

namespace mv {
VertexBuffer::VertexBuffer() { }

VertexBuffer::VertexBuffer(const void *data, GLuint size) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &id); }

void VertexBuffer::buffer_data(const void* data, GLuint size) {
    if (id == 0) glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, id); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

GLuint VertexBuffer::get_id() const { return id; }
} // namespace mv