#include "mv/scripting/register_types.h"

#include "mv/objects/node.h"
#include "mv/objects/sprite.h"
#include "mv/resource/texture.h"
#include "wrenbind17/wrenbind17.hpp"
#include "wrenbind17/stdmap.hpp"

#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/common.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/mat3x3.hpp>
#include <math.h>

namespace wren = wrenbind17;

namespace mv {

void register_math_types(wren::VM &vm) {
    wren::ForeignModule &module = vm.module("mv");

    auto &math = module.klass<WrenMath>("Math");

    math.funcStaticExt<static_cast<float (*)(float)>(&log10f)>("log10");
    math.funcStaticExt<&glm::modf<float>>("mod");
    math.funcStaticExt<static_cast<float (*)(float, float)>(&fmodf)>("fmod");
    math.funcStaticExt<&glm::smoothstep<float>>("smoothstep");
    math.funcStaticExt<&glm::step<float>>("step");
    math.funcStaticExt<&glm::fract<float>>("fract");
    math.funcStaticExt<&vec_mix<float>>("lerp"); // for the generic math function we can use lerp instead
}

void register_glm_types(wren::VM &vm) {
   wren::ForeignModule &module = vm.module("mv");

   auto &vec2 = module.klass<glm::vec2>("Vec2");
   vec2.ctor<float, float>();
   vec2.var<&glm::vec2::x>("x");
   vec2.var<&glm::vec2::y>("y");
   vec2.funcStaticExt<&glm::acos<2, float, glm::packed_highp>>("acos");
   vec2.funcStaticExt<&glm::acosh<2, float, glm::packed_highp>>("acosh");
   vec2.funcStaticExt<&glm::asin<2, float, glm::packed_highp>>("asin");
   vec2.funcStaticExt<&glm::asinh<2, float, glm::packed_highp>>("asinh");
   vec2.funcStaticExt<&glm::atanh<2, float, glm::packed_highp>>("atanh");
   vec2.funcStaticExt<&glm::cos<2, float, glm::packed_highp>>("cos");
   vec2.funcStaticExt<&glm::cosh<2, float, glm::packed_highp>>("cosh");
   vec2.funcStaticExt<&glm::sin<2, float, glm::packed_highp>>("sin");
   vec2.funcStaticExt<&glm::sinh<2, float, glm::packed_highp>>("sinh");
   vec2.funcStaticExt<&glm::tan<2, float, glm::packed_highp>>("tan");
   vec2.funcStaticExt<&glm::tanh<2, float, glm::packed_highp>>("tanh");
   vec2.funcStaticExt<&glm::abs<2, float, glm::packed_highp>>("abs");
   vec2.funcStaticExt<&glm::ceil<2, float, glm::packed_highp>>("ceil");
   vec2.funcStaticExt<&glm::length<2, float, glm::packed_highp>>("length");
   vec2.funcStaticExt<&glm::normalize<2, float, glm::packed_highp>>("normalize");
   vec2.funcStaticExt<&vec_mix<glm::vec2>>("mix");

   auto &vec3 = module.klass<glm::vec3>("Vec3");
   vec3.ctor<float, float, float>();
   vec3.var<&glm::vec3::x>("x");
   vec3.var<&glm::vec3::y>("y");
   vec3.var<&glm::vec3::z>("z");
   vec3.funcStaticExt<&glm::acosh<3, float, glm::packed_highp>>("acosh");
   vec3.funcStaticExt<&glm::asin<3, float, glm::packed_highp>>("asin");
   vec3.funcStaticExt<&glm::acos<3, float, glm::packed_highp>>("acos");
   vec3.funcStaticExt<&glm::asinh<3, float, glm::packed_highp>>("asinh");
   vec3.funcStaticExt<&glm::atanh<3, float, glm::packed_highp>>("atanh");
   vec3.funcStaticExt<&glm::cos<3, float, glm::packed_highp>>("cos");
   vec3.funcStaticExt<&glm::cosh<3, float, glm::packed_highp>>("cosh");
   vec3.funcStaticExt<&glm::sin<3, float, glm::packed_highp>>("sin");
   vec3.funcStaticExt<&glm::sinh<3, float, glm::packed_highp>>("sinh");
   vec3.funcStaticExt<&glm::tan<3, float, glm::packed_highp>>("tan");
   vec3.funcStaticExt<&glm::tanh<3, float, glm::packed_highp>>("tanh");
   vec3.funcStaticExt<&vec_mix<glm::vec3>>("mix");

   auto &vec4 = module.klass<glm::vec4>("Vec4");
   vec4.ctor<float, float, float, float>();
   vec4.var<&glm::vec4::x>("x");
   vec4.var<&glm::vec4::y>("y");
   vec4.var<&glm::vec4::z>("z");
   vec4.var<&glm::vec4::w>("w");
   vec4.funcStaticExt<&glm::acos<4, float, glm::packed_highp>>("acos");
   vec4.funcStaticExt<&glm::acosh<4, float, glm::packed_highp>>("acosh");
   vec4.funcStaticExt<&glm::asin<4, float, glm::packed_highp>>("asin");
   vec4.funcStaticExt<&glm::asinh<4, float, glm::packed_highp>>("asinh");
   vec4.funcStaticExt<&glm::atanh<4, float, glm::packed_highp>>("atanh");
   vec4.funcStaticExt<&glm::cos<4, float, glm::packed_highp>>("cos");
   vec4.funcStaticExt<&glm::cosh<4, float, glm::packed_highp>>("cosh");
   vec4.funcStaticExt<&glm::sin<4, float, glm::packed_highp>>("sin");
   vec4.funcStaticExt<&glm::sinh<4, float, glm::packed_highp>>("sinh");
   vec4.funcStaticExt<&glm::tan<4, float, glm::packed_highp>>("tan");
   vec4.funcStaticExt<&glm::tanh<4, float, glm::packed_highp>>("tanh");
   vec4.funcStaticExt<&vec_mix<glm::vec4>>("mix");

   auto &mat3 = module.klass<glm::mat3>("Mat3");
   mat3.ctor<float>();

   // TODO: extend as needed
}

void register_core_types(wren::VM &vm) {
    wren::ForeignModule &module = vm.module("mv");


}

void register_resource_types(wren::VM &vm) {
    wren::ForeignModule &module = vm.module("mv");

    auto &texture = module.klass<Texture>("Texture");

}

void register_node_types(wren::VM &vm) {
    wren::ForeignModule &module = vm.module("mv");

    auto &node = module.klass<Node>("Node");
    node.ctor<std::string>();
    node.var<&Node::children>("children");
    node.varReadonly<&Node::name>("name");
    node.func<&Node::add_instanced_child<Node>>("addChild");

    auto &sprite = module.klass<Sprite, Node>("Sprite");
    sprite.ctor<std::string>();
    // Node related stuff here
    sprite.var<&Sprite::children>("children");
    sprite.varReadonly<&Sprite::name>("name");
    sprite.func<&Sprite::add_instanced_child<Node>>("addChild");
    // Sprite props
    sprite.prop<&Sprite::get_pos, &Sprite::set_pos>("pos");
    sprite.prop<&Sprite::get_offset, &Sprite::set_offset>("offset");
    sprite.prop<&Sprite::get_scale, &Sprite::set_scale>("scale");
    sprite.prop<&Sprite::get_angle, &Sprite::set_angle>("angle");
    sprite.prop<&Sprite::get_angle_degrees, &Sprite::set_angle_degrees>("angle_degrees");
    sprite.prop<&Sprite::get_color, &Sprite::set_color>("color");
    sprite.prop<&Sprite::get_texture, &Sprite::set_texture>("texture");
    sprite.prop<&Sprite::is_centered, &Sprite::set_centered>("centered");
    // Node related stuff here


}

}
