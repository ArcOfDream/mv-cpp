#include "mv/graphics/indexbuffer.h"
#include "mv/gl.h"

namespace mv {
IndexBuffer::IndexBuffer(const void *data, GLuint size) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &id); }

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
} // namespace mv
