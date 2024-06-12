#include <epoxy/gl.h>

namespace mv {
class IndexBuffer {
    GLuint id;
    GLuint count;

  public:
    IndexBuffer(const void *data, GLuint count);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
};
} // namespace mv