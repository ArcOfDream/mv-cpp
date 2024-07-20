#include "mv/resource/material.h"
#include <memory>
#include <string>


namespace mv {

Material::Material(std::shared_ptr<Shader> _shd, std::string &_name) {
    shader = _shd;
    name = _name;

    type = MATERIAL;
}

void Material::update_uniforms() {}

// void Material::use() {}

///////////////////////////////////////////////////////////////////
// MaterialBuilder
///////////////////////////////////////////////////////////////////
MaterialBuilder::MaterialBuilder(std::string &name) {
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

std::shared_ptr<Material> MaterialBuilder::end() { return mat; }

}
