#include <glm/vec2.hpp>

#pragma once

namespace mv {
class Quad {
    int img_width;
    int img_height;
    glm::vec2 positions[4];
    glm::vec2 texcoords[4];

  public:
    Quad() {}
    Quad(float x, float y, float w, float h, float img_w, float img_h);
    glm::vec2* get_texcoords();
    void set_image_size(float w, float h);
    void set_viewport(float x, float y, float w, float h);
};

} // namespace mv
