#define GLM_ENABLE_EXPERIMENTAL
#include "mv/resource/material.h"
#include "mv/graphics/shader.h"
#include <utility>

#include <glm/gtx/string_cast.hpp>

namespace mv {
using std::pair;

Material::Material(std::shared_ptr<Shader> _shd, std::string _name) {
    shader = _shd;
    name = _name;

    type = MATERIAL;
}

void Material::update_uniforms() {
    int count = 0;
    for (auto u : uniforms) {
        switch (u.second.type) {
            case FLOAT:
                shader->set_float(u.second);
                break;
            case VEC2:
                shader->set_vec2(u.second);
                break;
            case SAMPLER2D:
                shader->set_int(u.second);
                {
                    if (const int* value = std::get_if<int>(&u.second.value)) {
                        glActiveTexture(GL_TEXTURE1 + count);
                        glBindTexture(GL_TEXTURE_2D, *value);
                        count++;
                    }
                }
                break;
            case INT:
                shader->set_int(u.second);
                break;
            case VEC3:
                shader->set_vec3(u.second);
                break;
            case VEC4:
                shader->set_vec4(u.second);
                break;
            case MAT3:
                shader->set_mat3(u.second);
                break;
            case BOOL:
                shader->set_bool(u.second);
                break;
            case MAT4:
                break;
            case AUTO:
                break;
        }
    }
}

void Material::use() { if(shader) shader->use(); }

// void Material::use() {}

///////////////////////////////////////////////////////////////////
// MaterialBuilder
///////////////////////////////////////////////////////////////////
MaterialBuilder::MaterialBuilder(std::string name) {
    mat = std::make_shared<Material>(nullptr, name);
}

MaterialBuilder &MaterialBuilder::begin(const std::string vs, const std::string fs) {
    mat->shader = std::make_shared<Shader>(vs, fs);
    return *this;
}

MaterialBuilder &MaterialBuilder::begin(std::shared_ptr<Shader> sh) {
    mat->shader = sh;
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_bool(const std::string name, const bool value) {
    mat->uniforms[name] = {
        name,
        BOOL,
        glGetUniformLocation(mat->shader->get_id(), name.c_str()),
        value
    };
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_int(const std::string name, const int value) {
    mat->uniforms[name] = {
        name,
        INT,
        glGetUniformLocation(mat->shader->get_id(), name.c_str()),
        value
    };
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_float(const std::string name, const float value) {
    mat->uniforms[name] = {
        name,
        FLOAT,
        glGetUniformLocation(mat->shader->get_id(), name.c_str()),
        value
    };
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_vec2(const std::string name, const glm::vec2 value) {
    mat->uniforms[name] = {
        name,
        VEC2,
        glGetUniformLocation(mat->shader->get_id(), name.c_str()),
        value
    };
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_vec3(const std::string name, const glm::vec3 value) {
    mat->uniforms[name] = {
        name,
        VEC3,
        glGetUniformLocation(mat->shader->get_id(), name.c_str()),
        value
    };
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_vec4(const std::string name, const glm::vec4 value) {
    mat->uniforms[name] = {
        name,
        VEC4,
        glGetUniformLocation(mat->shader->get_id(), name.c_str()),
        value
    };
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_mat3(const std::string name, const glm::mat3 value) {
    mat->uniforms[name] = {
        name,
        MAT3,
        glGetUniformLocation(mat->shader->get_id(), name.c_str()),
        value
    };
    
    return *this;
}

MaterialBuilder &MaterialBuilder::uniform_sampler2d(const std::string name, const int value) {
    mat->uniforms[name] = {
        name,
        SAMPLER2D,
        glGetUniformLocation(mat->shader->get_id(), name.c_str()),
        value
    };
    
    return *this;
}

std::shared_ptr<Material> MaterialBuilder::end() { return mat; }

} // namespace mv
