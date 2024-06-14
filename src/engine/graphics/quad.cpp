#include "mv/graphics/quad.h"
#include <glm/vec2.hpp>

namespace mv {

Quad::Quad(float x, float y, float w, float h, float img_w, float img_h) {
    img_width = img_w;
    img_height = img_h;

    set_viewport(x, y, w, h);
}

glm::vec2* Quad::get_texcoords() {
    return texcoords;
}

void Quad::set_image_size(float w, float h) {
    img_width = w;
    img_height = h;
}

void Quad::set_viewport(float x, float y, float w, float h) {
    // define corners going clockwise
    positions[0] = {0, 0};
    positions[1] = {w, 0};
    positions[2] = {w, h};
    positions[3] = {0, h};

    float inv_w = 1.0f / img_width;
    float inv_h = 1.0f / img_height;
    float w_tol = inv_w / 128.0f;
    float h_tol = inv_h / 128.0f;

    texcoords[0] = {x * inv_w + w_tol, y * inv_h + h_tol};
    texcoords[1] = {(x + w) * inv_w - w_tol, y * inv_h + h_tol};
    texcoords[2] = {(x + w) * inv_w - w_tol, (y + h) * inv_h - h_tol};
    texcoords[3] = {x * inv_w + w_tol, (y + h) * inv_h - h_tol};
}
}
