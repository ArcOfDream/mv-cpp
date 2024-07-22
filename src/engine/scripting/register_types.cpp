#define GLM_ENABLE_EXPERIMENTAL

#include "mv/scripting/register_types.h"

#include "mv/graphics/renderer.h"
#include "mv/graphics/vertex.h"
#include "mv/resource/material.h"
#include "mv/objects/node.h"
#include "mv/objects/sprite.h"
#include "mv/resource/texture.h"
#include "wrenbind17/wrenbind17.hpp"

#include <glm/common.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/easing.hpp>
#include <glm/gtx/exterior_product.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <math.h>

namespace wren = wrenbind17;

namespace mv {

// These are functions that are declared to make binding them easier,
// mostly GLM functions that aren't really able to be statically cast.
// TODO: move into util.h or make a new header file
float vec2_cross(glm::vec2 &x, glm::vec2 &y) { return glm::cross(x, y); }
float back_ease_in(float v) { return glm::backEaseIn(v); }
float back_ease_out(float v) { return glm::backEaseOut(v); }
float back_ease_inout(float v) { return glm::backEaseInOut(v); }
// back with overshoot param
float ex_back_ease_in(float v, float o) { return glm::backEaseIn(v, o); }
float ex_back_ease_out(float v, float o) { return glm::backEaseOut(v, o); }
float ex_back_ease_inout(float v, float o) { return glm::backEaseInOut(v, o); }

void register_math_types(wren::VM &vm) {
    wren::ForeignModule &module = vm.module("mv");

    auto &math = module.klass<WrenMath>("Math");

    math.funcStaticExt<static_cast<float (*)(float)>(&log10f)>("log10");
    math.funcStaticExt<&glm::modf<float>>("mod");
    math.funcStaticExt<static_cast<float (*)(float, float)>(&fmodf)>("fmod");
    math.funcStaticExt<&glm::smoothstep<float>>("smoothstep");
    math.funcStaticExt<&glm::step<float>>("step");
    math.funcStaticExt<&glm::fract<float>>("fract");
    // for the generic math function we can use lerp instead
    math.funcStaticExt<&vec_mix<float>>("lerp");
    // Easings, used with lerp/mix
    math.funcStaticExt<&glm::sineEaseIn<float>>("sineEaseIn");
    math.funcStaticExt<&glm::sineEaseOut<float>>("sineEaseOut");
    math.funcStaticExt<&glm::sineEaseInOut<float>>("sineEaseInOut");
    math.funcStaticExt<&glm::quinticEaseIn<float>>("quintEaseIn");
    math.funcStaticExt<&glm::quinticEaseOut<float>>("quintEaseOut");
    math.funcStaticExt<&glm::quinticEaseInOut<float>>("quintEaseInOut");
    math.funcStaticExt<&glm::quarticEaseIn<float>>("quartEaseIn");
    math.funcStaticExt<&glm::quarticEaseOut<float>>("quartEaseOut");
    math.funcStaticExt<&glm::quarticEaseInOut<float>>("quartEaseInOut");
    math.funcStaticExt<&glm::quadraticEaseIn<float>>("quadEaseIn");
    math.funcStaticExt<&glm::quadraticEaseOut<float>>("quadEaseOut");
    math.funcStaticExt<&glm::quadraticEaseInOut<float>>("quadEaseInOut");
    math.funcStaticExt<&glm::exponentialEaseIn<float>>("expoEaseIn");
    math.funcStaticExt<&glm::exponentialEaseOut<float>>("expoEaseOut");
    math.funcStaticExt<&glm::exponentialEaseInOut<float>>("expoEaseInOut");
    math.funcStaticExt<&glm::elasticEaseIn<float>>("elasticEaseIn");
    math.funcStaticExt<&glm::elasticEaseOut<float>>("elasticEaseOut");
    math.funcStaticExt<&glm::elasticEaseInOut<float>>("elasticEaseInOut");
    math.funcStaticExt<&glm::cubicEaseIn<float>>("cubicEaseIn");
    math.funcStaticExt<&glm::cubicEaseOut<float>>("cubicEaseOut");
    math.funcStaticExt<&glm::cubicEaseInOut<float>>("cubicEaseInOut");
    math.funcStaticExt<&glm::circularEaseIn<float>>("circEaseIn");
    math.funcStaticExt<&glm::circularEaseOut<float>>("circEaseOut");
    math.funcStaticExt<&glm::circularEaseInOut<float>>("circEaseInOut");
    math.funcStaticExt<&glm::bounceEaseIn<float>>("bounceEaseIn");
    math.funcStaticExt<&glm::bounceEaseOut<float>>("bounceEaseOut");
    math.funcStaticExt<&glm::bounceEaseInOut<float>>("bounceEaseInOut");
    math.funcStaticExt<&back_ease_in>("backEaseIn");
    math.funcStaticExt<&back_ease_out>("backEaseOut");
    math.funcStaticExt<&back_ease_inout>("backEaseInOut");
    math.funcStaticExt<&ex_back_ease_in>("exBackEaseIn");
    math.funcStaticExt<&ex_back_ease_out>("exBackEaseOut");
    math.funcStaticExt<&ex_back_ease_inout>("exBackEaseInOut");
}

void register_glm_types(wren::VM &vm) {
    // TODO: extend as needed
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
    vec2.funcStaticExt<&glm::length2<2, float, glm::packed_highp>>(
        "lengthSquared");
    vec2.funcStaticExt<&glm::normalize<2, float, glm::packed_highp>>(
        "normalize");
    vec2.funcStaticExt<&glm::angle<2, float, glm::packed_highp>>("angle");
    vec2.funcStaticExt<&glm::distance<2, float, glm::packed_highp>>(
        "distanceTo");
    vec2.funcStaticExt<&glm::distance2<2, float, glm::packed_highp>>(
        "distanceSquaredTo");
    vec2.funcStaticExt<&glm::dot<2, float, glm::packed_highp>>("dot");
    vec2.funcStaticExt<&vec2_cross>("cross");
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
}

void register_core_types(wren::VM &vm) {
    wren::ForeignModule &module = vm.module("mv");

    auto &renderer = module.klass<Renderer>("Renderer");
    renderer.funcStatic<&Renderer::get>("get");
    renderer.func<&Renderer::push_triangle>("pushTriangle");
    renderer.func<static_cast
    <void (Renderer::*)(Vertex&, Vertex&, Vertex&, Vertex&, unsigned int)>
    (&Renderer::push_quad)>("pushQuadVerts");

    auto &vertex = module.klass<Vertex>("Vertex");
    vertex.var<&Vertex::pos>("pos");
    vertex.var<&Vertex::uv>("uv");
    vertex.var<&Vertex::color>("color");

    auto &quad = module.klass<Quad>("Quad");
    quad.ctor<float, float, float, float, float, float>();
    quad.func<&Quad::set_image_size>("setImageSize");
    quad.func<&Quad::set_viewport>("setViewport");
    quad.func<&Quad::get_texcoords>("texcoords");

    auto &uniformbase = module.klass<UniformBase>("UniformBase");
    uniformbase.var<&UniformBase::name>("name");
    uniformbase.var<&UniformBase::type>("type");

    auto &shader = module.klass<Shader>("Shader");
}

void register_resource_types(wren::VM &vm) {
    wren::ForeignModule &module = vm.module("mv");

    auto &texture = module.klass<Texture>("Texture");
    texture.func<&Texture::get_tex_size>("getSize");
    texture.funcStaticExt<&load_texture_from_file>("loadFromFile");

    auto &material = module.klass<Material>("Material");
    material.var<&Material::uniforms>("uniforms");

    auto &materialbuilder = module.klass<MaterialBuilder>("MaterialBuilder");
    materialbuilder.ctor<std::string>();
    materialbuilder.func<static_cast<MaterialBuilder &(
        MaterialBuilder::*)(const std::string, const std::string)>(
        &MaterialBuilder::begin)>("begin");
    materialbuilder.func<&MaterialBuilder::uniform_bool>("uniform_bool");
    materialbuilder.func<&MaterialBuilder::uniform_int>("uniform_int");
    materialbuilder.func<&MaterialBuilder::uniform_float>("uniform_float");
    materialbuilder.func<&MaterialBuilder::uniform_vec2>("uniform_vec2");
    materialbuilder.func<&MaterialBuilder::uniform_vec3>("uniform_vec3");
    materialbuilder.func<&MaterialBuilder::uniform_vec4>("uniform_vec4");
    materialbuilder.func<&MaterialBuilder::uniform_mat3>("uniform_mat3");
    materialbuilder.func<&MaterialBuilder::end>("end");
}

void register_node_types(wren::VM &vm) {
    wren::ForeignModule &module = vm.module("mv");

    auto &node = module.klass<Node>("Node");
    node.ctor<wren::Variable, std::string>();
    node.var<&Node::children>("children");
    node.varReadonly<&Node::name>("name");
    node.func<&Node::add_instanced_child<Node>>("addChild");

    auto &sprite = module.klass<Sprite, Node>("Sprite");
    sprite.ctor<wren::Variable, std::string>();
    // Node related stuff here
    sprite.var<&Sprite::children>("children");
    sprite.varReadonly<&Sprite::name>("name");
    sprite.func<&Sprite::add_instanced_child<Node>>("addChild");
    sprite.func<&Sprite::add_instanced_child<Sprite>>("addChildSprite");
    // Sprite props
    sprite.prop<&Sprite::get_pos, &Sprite::set_pos>("pos");
    sprite.prop<&Sprite::get_offset, &Sprite::set_offset>("offset");
    sprite.prop<&Sprite::get_scale, &Sprite::set_scale>("scale");
    sprite.prop<&Sprite::get_angle, &Sprite::set_angle>("angle");
    sprite.prop<&Sprite::get_angle_degrees, &Sprite::set_angle_degrees>(
        "angleDegrees");
    sprite.prop<&Sprite::get_color, &Sprite::set_color>("color");
    sprite.prop<&Sprite::get_texture, &Sprite::set_texture>("texture");
    sprite.prop<&Sprite::is_centered, &Sprite::set_centered>("centered");
}

} // namespace mv
