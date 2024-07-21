#include "mv/resource/material.h"
#include "mv/graphics/shader.h"
#include <utility>

namespace mv {
using std::pair;

Material::Material(std::shared_ptr<Shader> _shd, std::string &_name) {
    shader = _shd;
    name = _name;

    type = MATERIAL;
}

void Material::update_uniforms() {
    for (pair<std::string, UniformBase> u : uniforms) {
        switch (u.second.type) {
            case AUTO:
                break;
            case BOOL:
                shader->set_bool(static_cast<ShaderUniform<bool>&>(u.second));
                break;
            case INT:
                shader->set_int(static_cast<ShaderUniform<int>&>(u.second));
                break;
            case FLOAT:
                shader->set_float(static_cast<ShaderUniform<float>&>(u.second));
                break;
            case VEC2:
                shader->set_vec2(static_cast<ShaderUniform<glm::vec2>&>(u.second));
                break;
            case VEC3:
                shader->set_vec3(static_cast<ShaderUniform<glm::vec3>&>(u.second));
                break;
            case VEC4:
                shader->set_vec4(static_cast<ShaderUniform<glm::vec4>&>(u.second));
                break;
            case MAT3:
                shader->set_mat3(static_cast<ShaderUniform<glm::mat3>&>(u.second));
                break;
            case MAT4:
                break;
            case SAMPLER2D:
                break;
            }
    }
}

// void Material::use() {}

///////////////////////////////////////////////////////////////////
// MaterialBuilder
///////////////////////////////////////////////////////////////////
MaterialBuilder::MaterialBuilder(std::string name) {
    mat = std::make_shared<Material>(nullptr, name);
}

MaterialBuilder &MaterialBuilder::begin(std::string &vs, std::string &fs) {
    mat->shader = std::make_shared<Shader>(vs, fs);
    return *this;
}

MaterialBuilder &MaterialBuilder::begin(std::shared_ptr<Shader> sh) {
    mat->shader = sh;
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_bool(std::string name, bool value) {
    ShaderUniform<bool> u{};
    u.name = name;
    u.type = BOOL;
    u.location = glGetUniformLocation(mat->shader->get_id(), name.c_str());
    u.value = value;
    
    mat->uniforms[name] = u;
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_int(std::string name, int value) {
    ShaderUniform<int> u{};
    u.name = name;
    u.type = INT;
    u.location = glGetUniformLocation(mat->shader->get_id(), name.c_str());
    u.value = value;
    
    mat->uniforms[name] = u;
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_float(std::string name, float value) {
    ShaderUniform<float> u{};
    u.name = name;
    u.type = FLOAT;
    u.location = glGetUniformLocation(mat->shader->get_id(), name.c_str());
    u.value = value;
    
    mat->uniforms[name] = u;
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_vec2(std::string name, glm::vec2 value) {
    ShaderUniform<glm::vec2> u{};
    u.name = name;
    u.type = VEC2;
    u.location = glGetUniformLocation(mat->shader->get_id(), name.c_str());
    u.value = value;
    
    mat->uniforms[name] = u;
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_vec3(std::string name, glm::vec3 value) {
    ShaderUniform<glm::vec3> u{};
    u.name = name;
    u.type = VEC3;
    u.location = glGetUniformLocation(mat->shader->get_id(), name.c_str());
    u.value = value;
    
    mat->uniforms[name] = u;
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_vec4(std::string name, glm::vec4 value) {
    ShaderUniform<glm::vec4> u{};
    u.name = name;
    u.type = VEC4;
    u.location = glGetUniformLocation(mat->shader->get_id(), name.c_str());
    u.value = value;
    
    mat->uniforms[name] = u;
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_mat3(std::string name, glm::mat3 value) {
    ShaderUniform<glm::mat3> u{};
    u.name = name;
    u.type = INT;
    u.location = glGetUniformLocation(mat->shader->get_id(), name.c_str());
    u.value = value;
    
    mat->uniforms[name] = u;
    
    return *this;
}

std::shared_ptr<Material> MaterialBuilder::end() { return mat; }

} // namespace mv
