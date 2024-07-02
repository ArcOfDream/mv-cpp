#include "resource.h"
#include "mv/graphics/shader.h"
#include <vector>
#include <unordered_map>

#pragma once

namespace mv {

class Material : public IResource {
    std::unordered_map<std::string, UniformBase> uniforms;
public:
    Shader &shader;

    Material(Shader, std::string);
    void add_uniform(std::string, UniformType);
    void update_uniforms();
    void use();
};
}