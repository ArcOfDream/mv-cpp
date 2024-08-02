#include "mv/gl.h"
#include "mv/resource/texture.h"
#include <memory>

#pragma once

namespace mv {
class FrameBuffer {
    std::shared_ptr<Texture> fb_texture;
    GLuint fbo_id = 0;
    GLuint rbo_id = 0;
    int width = 0;
    int height = 0;

  public:
    FrameBuffer();
    FrameBuffer(int, int);
    ~FrameBuffer();
    
    std::shared_ptr<Texture> get_texture();
};
} // namespace mv
