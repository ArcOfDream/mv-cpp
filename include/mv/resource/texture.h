#include "resource.h"
#include "mv/graphics/quad.h"
#include "mv/gl.h"
#include <glm/vec2.hpp>
#include <memory>

#pragma once

namespace mv {

class Texture : public IResource {
    unsigned int tex_id;
    Quad quad;
    glm::vec2 flip = {1, 1};
    glm::vec2 tex_size = {0, 0};
    GLenum format = GL_RGBA;
    GLenum flag_s = GL_REPEAT;
    GLenum flag_t = GL_REPEAT;
    GLenum filter_max = GL_NEAREST;
    GLenum filter_min = GL_NEAREST;

  public:
    Texture();
    Texture(std::string name);
    unsigned int get_id();
    glm::vec2 get_tex_size();
    Quad get_quad();
    void gen_id();
    void gen_with(const void *pixels, int width, int height, GLenum fmt);
    void bind();
    void unbind();

    std::shared_ptr<Texture> getptr() { return shared_from_base<Texture>(); }
};

std::shared_ptr<Texture> load_texture_raw(std::string name, int w, int h, GLenum format,
                         const void *pixels);
std::shared_ptr<Texture> load_texture_from_file(std::string name, std::string path);
std::shared_ptr<Texture> load_texture_from_source(std::string name, const void *bytes,
                                 unsigned long size);

} // namespace mv
