#include "mv/graphics/gl_debug.h"
#include <epoxy/gl.h>
#include <stdio.h>
#include <vector>

namespace mv {
void error_check(const char *label) {
    GLenum err = glGetError();

    if (err != GL_NO_ERROR) {
        printf("GL Error at %s! ", label);
        switch (err) {
        case GL_INVALID_ENUM:
            printf("Invalid Enum\n");
            break;
        case GL_INVALID_VALUE:
            printf("Invalid Value\n");
            break;
        case GL_INVALID_OPERATION:
            printf("Invalid Operation\n");
            break;
        case GL_OUT_OF_MEMORY:
            printf("Out of memory\n");
            break;
        }
    }
}

void gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                         GLsizei length, const GLchar *message,
                         const void *userParam) {
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
            severity, message);
    gl_messages.push_back(
        {source, type, id, severity, length, message, userParam});
}
} // namespace mv