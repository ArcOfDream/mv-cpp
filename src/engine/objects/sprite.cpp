#include "sol/sol.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES

#include "mv/objects/sprite.h"
#include "mv/graphics/graphics.h"
#include "mv/util.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <memory>
#include <string>
#include <math.h>

namespace mv {
Sprite::Sprite(const char* _name, std::shared_ptr<Texture> _tex) : Node(_name) {
    name = _name;
    set_texture(_tex);
    // set_color(color);
}

Sprite::Sprite(sol::this_state lua, const char* _name, std::shared_ptr<Texture> _tex) : Node(lua, _name) {
    name = _name;
    set_texture(_tex);
    // set_color(color);
}

void Sprite::update_vertex_pos(glm::vec2 pos, glm::vec2 size) {
    verts[0].pos = {pos.x, pos.y};
    verts[1].pos = {pos.x + size.x, pos.y};
    verts[2].pos = {pos.x + size.x, pos.y + size.y};
    verts[3].pos = {pos.x, pos.y + size.y};
}

void Sprite::flag_dirty(bool apply_children) {
    dirty = true;

    if (apply_children)
        for (auto &child : children) {
            if (Sprite *spr = dynamic_cast<Sprite *>(child.get())) {
                spr->flag_dirty();
            }
        }
}

glm::vec2 Sprite::get_pos() const { return pos; }
void Sprite::set_pos(glm::vec2 value) {
    pos = value;
    flag_dirty();
}

glm::vec2 Sprite::get_offset() const { return offset; }
void Sprite::set_offset(glm::vec2 value) {
    offset = value;
    flag_dirty();
}

glm::vec2 Sprite::get_scale() const { return scale; }
void Sprite::set_scale(glm::vec2 value) {
    scale = value;
    flag_dirty();
}

float Sprite::get_angle() const { return angle; }
void Sprite::set_angle(float value) {
    angle = value;
    angle_degrees = (float)(value * M_PI / 180.0f);;
    flag_dirty();
}

float Sprite::get_angle_degrees() const { return angle_degrees; }
void Sprite::set_angle_degrees(float value) {
    angle_degrees = value;
    angle = (float)(value * 180.0f / M_PI);
    flag_dirty();
}

glm::vec4 Sprite::get_color() const { return color; }
void Sprite::set_color(glm::vec4 value) {
    // since we can simply update the vertex data without rebuilding
    // the transform matrix, this doesn't affect the dirty bit
    color = value;

    // update vertices to hold the values as well
    for (Vertex &v : verts) {
        v.color = value;
    }
}

std::shared_ptr<Texture> Sprite::get_texture() { return tex; }
void Sprite::set_texture(std::shared_ptr<Texture> value) {
    tex = value;

    // if we do this to unset the texture from the object, then
    // we can skip updating anything else and exit early
    if (value == nullptr)
        return;

    flag_dirty();

    // we will go ahead and update the texture coordinates on the spot
    glm::vec2 *coords = tex->get_quad().get_texcoords();
    for (int i = 0; i < 4; i++) {
        verts[i].uv = coords[i];
    }
}

bool Sprite::is_centered() const { return center_image; }
void Sprite::set_centered(bool value) {
    center_image = value;
    flag_dirty(false);
}

void Sprite::rebuild_transform() {
    // setup model matrix
    glm::mat3 model{ 1.0f };

    model = glm::translate(model, pos);
    model = glm::rotate(model, angle);
    model = glm::scale(model, scale);

    transform = model;

    if (parent)
        if (Sprite *p = dynamic_cast<Sprite *>(parent)) {
            transform = p->transform * transform;
        }

    apply_transform();
}

void Sprite::apply_transform() {
    glm::vec2 size{0, 0};
    glm::vec2 offs = offset;
    if (tex) {
        size = tex->get_tex_size();
        if (center_image)
            offs -= size * 0.5f;
    }

    update_vertex_pos(offs, size);
    // apply matrix to positions
    for (auto &v : verts) {
        auto result = transform * glm::vec3(v.pos, 1);
        v.pos = {result.x, result.y};
    }
}

void Sprite::_draw() {
    if (dirty) {
        rebuild_transform();
        // apply_transform();
        dirty = false;
    }

    if (tex)
        Renderer::get().push_quad(verts[0], verts[1], verts[2], verts[3], tex->get_id());

    for (auto &child : children) {
        child->_draw();
    }
    draw();
}
} // namespace mv
