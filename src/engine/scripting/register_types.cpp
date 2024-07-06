#include "mv/scripting/register_types.h"
#include "sol/sol.hpp"

#include "mv/objects/node.h"
#include "mv/objects/sprite.h"
#include "mv/resource/texture.h"

#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <memory>

namespace mv {

void register_glm_types(sol::state &lua) {
    lua.new_usertype<glm::vec2>("vec2",
    sol::constructors<
        glm::vec2(), 
        glm::vec2(float),
        glm::vec2(float, float)>(),
    sol::call_constructor, [](float x, float y) { return glm::vec2{x, y}; },
    "x", &glm::vec2::x,
    "y", &glm::vec2::y);

    lua.new_usertype<glm::vec3>("vec3",
    sol::constructors<
        glm::vec3(), 
        glm::vec3(float), 
        glm::vec3(float, float, float),
        glm::vec3(glm::vec2, float), 
        glm::vec3(float, glm::vec2)>(),
    sol::call_constructor, [](float x, float y, float z) { return glm::vec3{x, y, z}; },
    "x", &glm::vec3::x,
    "y", &glm::vec3::y,
    "z", &glm::vec3::z);

    lua.new_usertype<glm::vec4>("vec4",
    sol::constructors<
        glm::vec4(), 
        glm::vec4(float), 
        glm::vec4(float, float, float, float),
        glm::vec4(glm::vec2, float, float),
        glm::vec4(float, glm::vec2, float),
        glm::vec4(float, float, glm::vec2),
        glm::vec4(glm::vec2, glm::vec2),
        glm::vec4(glm::vec3, float),
        glm::vec4(float, glm::vec3)>(),
    sol::call_constructor, [](float x, float y, float z, float w) { return glm::vec4{x, y, z, w}; },
    "x", &glm::vec4::x,
    "y", &glm::vec4::y,
    "z", &glm::vec4::z,
    "w", &glm::vec4::w);

    lua.new_usertype<glm::mat3>("mat3",
    sol::constructors<
        glm::mat3(),
        glm::mat3(float)
    >());
}

void register_resource_types(sol::state &lua) {
    lua.new_usertype<Texture>("Texture",
    sol::constructors<Texture(const char*)>());

    lua.set_function("load_texture_from_source", load_texture_from_source);
    lua.set_function("load_texture_from_file", load_texture_from_file);
    lua.set_function("load_texture_raw", load_texture_raw);
}

void register_node_types(sol::state &lua) {
    lua.new_usertype<Node>("Node",
    sol::constructors<Node(const char*)>(),
    "init", &Node::init,
    "update", &Node::update,
    "draw", &Node::draw,
    "input", &Node::input,
    "add_instanced_child", [](Node& node, Node *child) {
        return node.add_instanced_child<Node>(child);
    });

    lua.new_usertype<Sprite>("Sprite",
    sol::constructors<Sprite(const char*), Sprite(const char*, std::shared_ptr<Texture>)>(),
    sol::base_classes, sol::bases<Node>(),
    // "init", &Sprite::init,
    // "update", &Sprite::update,
    // "draw", &Sprite::draw,
    // "input", &Sprite::input,

    "pos", sol::property(&Sprite::get_pos, &Sprite::set_pos),
    "offset", sol::property(&Sprite::get_offset, &Sprite::set_offset),
    "scale", sol::property(&Sprite::get_scale, &Sprite::set_scale),
    "angle", sol::property(&Sprite::get_angle, &Sprite::set_angle),
    "angle_degrees", sol::property(&Sprite::get_angle_degrees, &Sprite::set_angle_degrees),
    "texture", sol::property(&Sprite::get_texture, &Sprite::set_texture),
    "color", sol::property(&Sprite::get_color, &Sprite::set_color),
    
    "set_pos", &Sprite::set_pos,
    "get_pos", &Sprite::get_pos,
    "set_offset", &Sprite::set_offset,
    "get_offset", &Sprite::get_offset,
    "set_scale", &Sprite::set_scale,
    "get_scale", &Sprite::get_scale,
    "set_angle", &Sprite::set_angle,
    "get_angle", &Sprite::get_angle,
    "set_angle_degrees", &Sprite::set_angle_degrees,
    "get_angle_degrees", &Sprite::get_angle_degrees,
    "set_texture", &Sprite::set_texture,
    "get_texture", &Sprite::get_texture,
    "set_color", &Sprite::set_color,
    "get_color", &Sprite::get_color,
    "is_centered", &Sprite::is_centered,
    "set_centered", &Sprite::set_centered);
}

}