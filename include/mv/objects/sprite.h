#include "mv/graphics/vertex.h"
#include "mv/objects/node.h"
#include "mv/resource/texture.h"
#include "sol/sol.hpp"
#include <glm/glm.hpp>
#include <memory>
// #include <string>

#pragma once

namespace mv {
class Sprite : public Node {
    glm::vec2 pos = {0, 0};
    glm::vec2 offset = {0, 0};
    glm::vec2 scale = {1, 1};
    float angle = 0; // radians by default
    float angle_degrees = 0;

    glm::vec4 color = {1, 1, 1, 1};

    std::shared_ptr<Texture> tex = nullptr;
    bool center_image = true;

    // bool to determine when to rebuild the matrix
    bool dirty = false;
    glm::mat3 transform = glm::mat3(1.0f);
    Vertex verts[4] = {};

    inline void update_vertex_pos(glm::vec2, glm::vec2);


  public:
    // ctor
    using Node::Node;
    Sprite(const char*, std::shared_ptr<Texture>);
    Sprite(sol::this_state, const char*, std::shared_ptr<Texture>);

    void flag_dirty(bool = true);

    void _init() {};
    // void _update(double);
    void _draw();
    void _input(SDL_Event&){};

    virtual void init() {};
    virtual void update(double) {};
    virtual void draw() {};
    virtual void input(SDL_Event&) {};

    glm::vec2 get_pos() const;
    void set_pos(glm::vec2);

     glm::vec2 get_offset() const;
    void set_offset(glm::vec2);

    glm::vec2 get_scale() const;
    void set_scale(glm::vec2);

    float get_angle() const;
    void set_angle(float);

    float get_angle_degrees() const;
    void set_angle_degrees(float);

    glm::vec4 get_color() const;
    void set_color(glm::vec4);

    std::shared_ptr<Texture> get_texture();
    void set_texture(std::shared_ptr<Texture>);

    bool is_centered() const;
    void set_centered(bool);

    void rebuild_transform();
    void apply_transform();
};
} // namespace mv
