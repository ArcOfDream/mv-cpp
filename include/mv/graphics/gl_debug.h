#include <epoxy/gl.h>
#include <epoxy/gl_generated.h>
#include <vector>

#pragma once

namespace mv {
struct GLMessage {
    GLenum source;
    GLenum type;
    GLuint id;
    GLenum severity;
    GLsizei length;
    const GLchar *message;
    const void *user_data;
};

static std::vector<GLMessage> gl_messages = {};

void error_check(const char *label);
void gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                         GLsizei length, const GLchar *message,
                         const void *userParam);
} // namespace mv
