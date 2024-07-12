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
    // glm::vec2
    lua.new_usertype<glm::vec2>("Vec2",
    sol::constructors<
        glm::vec2(), 
        glm::vec2(float),
        glm::vec2(float, float)>(),
    sol::call_constructor, [](float x, float y) { return glm::vec2{x, y}; },
    "x", &glm::vec2::x,
    "y", &glm::vec2::y);
    
    // glm::vec3
    lua.new_usertype<glm::vec3>("Vec3",
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
    
    // glm::vec4
    lua.new_usertype<glm::vec4>("Vec4",
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
    
    // glm::mat3
    lua.new_usertype<glm::mat3>("Mat3",
    sol::constructors<
        glm::mat3(),
        glm::mat3(float)
    >());
    
    // TODO: expose more of GLM as needed
}

void register_resource_types(sol::state &lua) {
    // mv:Texture
    lua.new_usertype<Texture>("Texture",
    sol::constructors<Texture(const char*)>());
    
    // TODO: expose mv::Material when ready 
    // and provide a builder to build shaders from source

    lua.set_function("load_texture_from_source", load_texture_from_source);
    lua.set_function("load_texture_from_file", load_texture_from_file);
    lua.set_function("load_texture_raw", load_texture_raw);
}

void register_node_types(sol::state &lua) {
    // mv::Node
    lua.new_usertype<Node>("Node",
    sol::constructors<Node(sol::this_state, const char*)>(),
    sol::meta_function::index, &Node::lua_dynamic_get,
    sol::meta_function::new_index, &Node::lua_dynamic_set,
    sol::meta_function::length, [](Node& n) { return n.entries.size(); },
    "init", &Node::lua_init,
    // "update", &Node::lua_update,
    "update", sol::property(
        [](Node &node) { return node.lua_update; },
        [](Node &node, sol::function func) { node.lua_update = func; }
    ),
    "draw", &Node::lua_draw,
    "input", &Node::lua_input,
    
    // NOTE: Likely need a better way to expose this
    "children", &Node::children,
    
    // I probably don't actually need to do it like this, and just have 
    // the Node template exposed.
    "add_instanced_child", sol::overload(
        &Node::add_instanced_child<Sprite>,
        &Node::add_instanced_child<Node>
    )
    );

    // mv::Sprite
    lua.new_usertype<Sprite>("Sprite",
    sol::constructors<
        Sprite(sol::this_state, const char*, std::shared_ptr<Texture>),
        Sprite(sol::this_state, const char*)
    >(),
    sol::base_classes, sol::bases<Node>(),
    sol::meta_function::index, &Sprite::lua_dynamic_get,
    sol::meta_function::new_index, &Sprite::lua_dynamic_set,
    sol::meta_function::length, [](Sprite& n) { return n.entries.size(); },

    "pos", sol::property(&Sprite::get_pos, &Sprite::set_pos),
    "offset", sol::property(&Sprite::get_offset, &Sprite::set_offset),
    "scale", sol::property(&Sprite::get_scale, &Sprite::set_scale),
    "angle", sol::property(&Sprite::get_angle, &Sprite::set_angle),
    "angle_degrees", sol::property(&Sprite::get_angle_degrees, &Sprite::set_angle_degrees),
    "texture", sol::property(&Sprite::get_texture, &Sprite::set_texture),
    "color", sol::property(&Sprite::get_color, &Sprite::set_color),
    "centered", sol::property(&Sprite::is_centered, &Sprite::set_centered),
    
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
    "set_centered", &Sprite::set_centered
    );
}

}